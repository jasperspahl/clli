//
// Created by jasper on 01.02.23.
//

#include "get_testdata.h"
#include <stdlib.h>

opensource_project **get_testdata_set(void) {
	opensource_project **ocp_arr = malloc(sizeof(opensource_project *) * 20);
	ocp_arr[0] = create_opensource_project("Linux", "Linux is a open source os Kernel", "https://git.kernel.org", 0, 0);
	ocp_arr[1] = create_opensource_project("gh", "Github Commandline Interface", "https://github.com/cli/cli", 31165,
	                                       385);
	ocp_arr[2] = create_opensource_project("jiraWorklog",
	                                       "A small python programs to summarize the workhours of the last or current sprint",
	                                       "https://github.com/jasperspahl/jiraWorklog", 1, 0);
	ocp_arr[3] = create_opensource_project("jasperspahl/dwm",
	                                       "dwm is an extremely fast, small, and dynamic window manager for X.",
	                                       "https://github.com/jasperspahl/dwm", 2, 0);
	ocp_arr[4] = create_opensource_project("penrose",
	                                       "Penrose is a modular library for configuring your own X11 window manager in Rust.\n\nThis means that, unlike most other tiling window managers, Penrose is not a binary that you install on your system. Instead, you use it like a normal dependency in your own crate for writing your own window manager. Don't worry, the top level API is well documented and a lot of things will work out of the box, and if you fancy digging deeper you'll find lots of opportunities to customise things to your liking.",
	                                       "https://github.com/sminez/penrose", 999, 12);
	ocp_arr[5] = create_opensource_project("cobra", "A Commander for modern Go CLI interactions ",
	                                       "https://github.com/spf13/cobra", 30527, 113);
	ocp_arr[6] = create_opensource_project("lazygit",
	                                       "A simple terminal UI for git commands, written in Go with the `gocui` library.",
	                                       "https://github.com/jesseduffield/lazygit", 32436, 393);
	ocp_arr[7] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[8] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[9] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[10] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[11] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[12] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[13] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[14] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[15] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[16] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[17] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[18] = create_opensource_project("Projekt", "A simple project management tool", "", 0, 0);
	ocp_arr[19] = NULL;
	return ocp_arr;
}
