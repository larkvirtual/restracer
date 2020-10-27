-- $Id: session_manager_pkg.adb 235 2004-07-11 08:43:12Z alter $
with Ada.Exceptions;   use Ada.Exceptions;

package body Session_Manager_Pkg is

protected body Session_Manager is

    procedure	Init is
    begin
	for i in In_Usage'Range loop
	    In_Usage(i) := false;
	end loop;
    end;
    
    procedure	Get_Session_ID(Session_ID : out Positive) is
    found : boolean := false;
    begin
	for i in In_Usage'Range loop
	    if In_Usage(i) = false then
	    begin
		In_Usage(i) := true;
		Session_ID := i;
		found := true;
		exit;
	    end;
	    end if;
	end loop;
	if found = false then Raise_Exception (Constraint_Error'Identity,
	    "No more room");
	end if;
    end;

    procedure	Release_Session_ID(Session_ID : in Positive) is
    begin
	if In_Usage(Session_ID) = false then
	    Raise_Exception (Constraint_Error'Identity,
	            	    "Trying to Release unlocked value");
	end if;
	In_Usage(Session_ID) := false;
    end;

    function	Sessions_In_Use return Natural is
    count : Natural := 0;
    begin
	for i in In_Usage'Range loop
	    if In_Usage(i) = true then
	    count := count + 1;
	    end if;
	end loop;
	return count;
    end;

end Session_Manager;

end Session_Manager_Pkg;
