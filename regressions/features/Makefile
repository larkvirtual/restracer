# Dirs descriptions
# 0* - manual nonautostart tracing testing dirs
# 1* - autostart tracing testing dirs

info:
	@echo "Target: tracefile-method networked-method"

tracefile-method: clean usleep
	./build-n-run-tracefile.sh

networked-method: clean usleep
	./build-n-run-networked.sh
	@echo "*******************************************************************"
	@echo "* On slow machine tune argument for ./usleep in"
	@echo "* build-n-run-networked.sh script."
	@echo "* For example, try to set ./usleep variables repGenStartupTime,"
	@echo "* repGenShutdownTime into 32 or 64 value."
	@echo "*******************************************************************"

usleep: usleep.c
	$(CC) -o usleep usleep.c

clean: clean-dirs
	@rm -f usleep

clean-dirs:
	@for i in `find -type d | tail -n +2`; do \
rm -f $$i/.tracefile.out; \
rm -f $$i/main; \
rm -f $$i/main.o; \
rm -f $$i/template.tmp; \
rm -f $$i/tracefile.err; \
rm -f $$i/tracefile.out; \
rm -f $$i/.real; \
done
