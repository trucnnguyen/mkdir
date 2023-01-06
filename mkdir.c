/******************************************************************************
 * Truc Nguyen
 * ID: 016465020
 * CS2600, Fall 2022
 * Program Assignment 04
 * This assignment is to write a version of "mkdir" command in Linux/Unix.
 
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int mkdir(const char *pathname, mode_t mode);
int main(int argc, char *argv[])
{
    char *s2, *c, *s;
    struct stat sb;
    int k, len, pE, temp;
    k = pE = 0;
    
    
    //Check for user input if correct or not.
    if(strcmp(argv[1], "mkdir"))
    {
        printf("Wrong syntax, please try again!\n./fmkdir mkdir [OPTION] ...\n");
        exit(EXIT_FAILURE);
    }
    
    /*-------------------------------------------------------------------------------------------------------------------
        User input is correct, now check if -p is in user's input.
            * If it is not, mkdir will take care of whether the parent directory is created or not, if not it will signal.
            * If "-p" presents, system will create parent dir if it does not exist and then create child dir. 
            System will then change pE to TRUE (1) to indicate.
    ----------------------------------------------------------------------------------------------------------------------*/
    for(int j = 1; j < argc; j++)
    {
        if(strcmp(argv[j], "-p") == 0)
        {
            pE = 1;
        }
    }
    
/*************************************************************************************************************************
    "-p" exists -> System will loop to find the string contains dir to create.
        * We do this by check argv[] if the string contain "-p",
        string that does not contain it after mkdir is the dir string to create.
        * System will store the string contains dir name into s and create another string with same length called "p".
        * System will check in s if s contains '/'. 
            -> If it does not, system will simply create a dir under that name. 
            -> If it does contain '/', below steps will be taken.
        * String p will contain the break down of dir name.
        * For ex: 
            dir from user's input is a/b/c, p will contain:
            a
            a/b
            a/b/c
        * Since string in C is NULL terminator, the last element in p will be assigned to NULL to avoid garbage.
    In string s, system will divide by '/'. Meaning, p will contain the name up tp '/' 
    and create a dir by mkdir under that name.
    System will keep looping until p reached NULL.
        
******************************************************************************************************************************/
    if(pE == 1) 
    {
        //Loop to search for directory name
        for(int l = 2; l < argc; l++)
        {
            c = strstr(argv[l], "-p");
            if (c == NULL)
                s = argv[l];
        }

        len = strlen(s);
        char p[len]; 
        memset(p, 0, len);
        s2= strchr(s, '/'); 

        /*----------------------------------------------
            There is no '/' in dir entry from user.
            Use this entry s as directory name to create.
        ------------------------------------------------*/
        if(s2 == NULL)
            temp = mkdir(s, 0755) == 0;
        
        /*------------------------------------------------------------
            There is '/'.
            System will create a directory with name separated by '/' 
            and keep looping.
            -> Sytem will keep looping to create the directory.  
        -------------------------------------------------------------*/
       else
       {
        for(int i = 0; i < len; i++)
        {
            if(s[i] == '/')
                temp = mkdir(p, 0755) == 0;

            p[k] = s[i];
            if(k < len - 1)
                k++;
        }
            p[len] = '\0';
            temp = mkdir(p, 0755) == 0;
       }
    }
    
/************************************************************************************
 -p does not exist -> Systemw will signal error message when directory is being duplicated.
    * We do this by first check if the directory from user's input is already created or not.
        * If it is, system will displays error message and not allow user from creating it.
        * It is is not, system will check if user's input contain '/' or not.
            - If entry does not contain '/', system will create the dir.
            - If entry does contain '/', the following will take action: 
                * System will check if dir before '/' is created. If is is, system will create the child dir under that par dir.
                If it is not, system will display error message stating that the par dir does not exist. 
************************************************************************************/
    else if(pE == 0)
    {
        s = argv[2];
        s2= strchr(s, '/');
        len = strlen(s);
        char p[len];
        memset(p, 0, len);

        if(stat(s, &sb) == 0 && S_ISDIR(sb.st_mode))
        {                 
            printf("mkdir: cannot create directory '%s': File already existed\n", s);
            exit(EXIT_FAILURE);
        }

        if(s2 != NULL) // '/' exists 
        {
            for(int i = 0; i < len; i++)
            {
                if(s[i] == '/')
                {
                    if(stat(p, &sb) == 0 && S_ISDIR(sb.st_mode))
                        temp = mkdir(p, 0755);
                    else
                        printf("mkdir: cannot create directory '%s': No such file or directory\n", s);
                }

                p[k] = s[i];
                if(k < len - 1)
                    k++;
            }

            p[len] = '\0';
            temp = mkdir(p, 0755);
        }
        /*----------------------------------------------
            There is no '/' in dir entry from user.
            Use this entry s as directory name to create.
        ------------------------------------------------*/
        else if(s2 == NULL) 
            temp = mkdir(s, 0755);
    }
    return 0;
}

