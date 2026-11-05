#ifndef DATACHECK_H
#define DATACHECK_H

/*
** Checks that the game data has been installed before the game starts loading it. Each entry of
** "required" must be reachable through the file search paths, and "disc_file" must be inside at
** least one of "disc_dirs" (folders under the user, data or program path; "." is the path itself).
** Both lists end with nullptr. On failure the missing files are shown in a message box and false
** is returned.
*/
bool Check_Game_Data(const char* game_name,
                     const char* const required[],
                     const char* disc_file,
                     const char* const disc_dirs[]);

#endif /* DATACHECK_H */
