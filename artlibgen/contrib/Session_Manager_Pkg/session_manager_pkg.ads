-- $Id: session_manager_pkg.ads 236 2004-07-11 08:51:53Z alter $
package Session_Manager_Pkg is

type Task_Activity is array (1..100) of boolean;

    protected Session_Manager is

	procedure	Init;
        procedure	Get_Session_ID(Session_ID : out Positive);
        procedure	Release_Session_ID(Session_ID : in Positive);
        function	Sessions_In_Use return Natural;

    private
	In_Usage : Task_Activity;

    end Session_Manager;

end Session_Manager_Pkg;
