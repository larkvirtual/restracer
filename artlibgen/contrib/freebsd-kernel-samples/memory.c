#include <sys/types.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/malloc.h>

MALLOC_DECLARE (M_MEM);
MALLOC_DEFINE (M_MEM, "memory", "buffer for array");
static int event_handler (struct module *module, int event, void *arg)
{
	int e=0;
	static int *array,i=0;
	switch (event)
	{
		case MOD_LOAD:
			uprintf("Begin\n");
			array=malloc(40,M_MEM,M_NOWAIT);
			if (array==NULL)
			{
				uprintf ("Unable to allocatre memory\n");
				return 15;
			}
			for (i=0;i<10;i++)
			{
				array[i]=i;
				uprintf ("%d \n",array[i]);
			}
			break;
		case MOD_UNLOAD:
			uprintf("End\n");
			for (i=0;i<10;i++)
				uprintf ("%d \n",array[i]);
			free(array,M_MEM);
			break;
		default:
			e=EOPNOTSUPP;
			break;
	}
	return (e);
}
static moduledata_t memory_conf = {
	"memory_work",
	event_handler,
	NULL
};
DECLARE_MODULE(memory_work, memory_conf, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
