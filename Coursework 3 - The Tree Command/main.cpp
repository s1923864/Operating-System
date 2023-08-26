/*
 * The Tree Command
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 3
 */

#include <infos.h>



/*
* @global SIGN A number of concatenation(s) of "    "  and  "|   ".
* @global SIGN_SIZE The size of global variable SIGN.
* @global PATH The path for file system.
* @global SIZE The size of global variable of PATH. 
* @global DIR_NUM The number of directories under a specific path.
* @global FILE_NUM The number of files under a specific path.
*/
char *SIGN;
int SIGN_SIZE = 0;
static char *PATH;
static int SIZE = 0;
static int DIR_NUM = 0;
static int FILE_NUM = 0;





/** Given a char pointer destination, a char pointer source, the start index of copy, the end index of copy and an order, returns char pointer destination with copy of char pointer source from start index to end index.
* @param dest a char pointer destination.
* @param src a char pointer source.
* @param start the start index of copy.
* @param end the end index of copy. 
* @return Returns a char pointer destination.
*/
char *strcpy(char *dest, char *src, int start, int end){
const char *c = src;
char *d = dest;
int index = start;
c += start;
d += start;
while(index <= end){
*d = *c;
c++;
d++;
index++;
}
*d = '\0';
return dest;

}
char *substr(int i,char *s){

char *tmp = s;

tmp += i;

return tmp;
}




/** concate global variable PATH with parameter file_name
* @param file_name name of the file or directory.
*/
void strcat(const char *file_name){

char *tmp = PATH;
const char *tmp_f = file_name;
tmp += SIZE;
if(SIZE != 1){
*tmp = '/';
tmp++;
SIZE++;
}
while(*tmp_f != '\0'){
*tmp = *tmp_f;
tmp++;
tmp_f++;
SIZE++;
}
*tmp = '\0';

}




/** concate global variable SIGN with parameter sign
* @param sign a sign coulbe either "    " or "|   ".
*/
void strcat_sign(const char *sign){

char *tmp = SIGN;
const char *tmp_f = sign;
tmp += SIGN_SIZE;

while(*tmp_f != '\0'){
*tmp = *tmp_f;
tmp++;
tmp_f++;
SIGN_SIZE++;
}
*tmp = '\0';
}




/** split global variable PATH at given index.
* @param length a given index to split PATH.
*/
void strsplit(int length){
char *tmp = PATH;
tmp += length;
*tmp = '\0';
tmp++;
SIZE--;
while(*tmp != '\0'){
*tmp = NULL;
tmp++;
SIZE--;
}
*tmp = NULL;
}




/** split global variable SIGN at given index.
* @param length a given index to split SIGN.
*/
void strsplit_sign(int length){
char *tmp = SIGN;
tmp += length;
*tmp = '\0';
tmp++;
SIGN_SIZE--;
while(*tmp != '\0'){
*tmp = NULL;
tmp++;
SIGN_SIZE--;
}
*tmp = NULL;
}




/** Identify whether a char is in a range. For example identify whether a char 'a' is in the pattern (a-j).
* @param a the start range.
* @param b the end range.
* @param d the target char used to identify whether it is in range.
* @return return true if d is in range from a to b,Otherwise return false.
*/
bool range(char a, char b, char d){

return (d >= a) && (d <= b);
}




/** Identify whether a char is in a range. For example, identify whether a  char 'a' is in the pattern (abcdgj).
* @param a the start index of p.
* @param b the end index of p.
* @param p the pattern.
* @param s the char to match the pattern.
* @return return true if d is in range from a to b,Otherwise return false.
*/
bool specific_range(int a, int b, char *p, char s){


for(int i = a; i <= b ; i++){

if(s == p[i]){

return true;

}

}
return false;

}




/** Identify whether a string matches a given pattern. For example, identify whether a string "apoiub" matches the pattern (a-m)*(ab).
* @param s the string to match the pattern.
* @param p the pattern.
* @return return true if s matches p ,Otherwise return false.
*/
bool is_Match(char *s, char *p){

int lens = strlen(s);
int lenp = strlen(p);

if(lenp == 0) return lens == 0;
if (lenp == 1){
   if(lens == 0)
      return false;
   if(lens != 0 && (p[0] == s[0]))
      return is_Match(substr(1,s), substr(1,p)); 
   else return false;

}

if(lenp >=2 ){
if(p[0] == '('){
if(p[2] == '-'){

if(lenp >= 5){
if(lenp >=6){      
if(p[5] == '*'){
while(strlen(s) != 0 && range(p[1],p[3],s[0])){
if(is_Match(s,substr(6,p)))
return true;
else{

s = substr(1,s);

}
}
return is_Match(s,substr(6,p));
}
else if(p[5] == '?'){
if(range(p[1],p[3],s[0])){
return is_Match(s,substr(6,p)) || is_Match(substr(1,s),substr(6,p));
}
else{
return is_Match(s,substr(6,p));
}

}

else{
if(range(p[1],p[3],s[0]))
return is_Match(substr(1,s),substr(5,p));
else
return false;
}

}

}


}

else{
int k = 0;
for(;p[k]!=')';k++);

if(lenp > (k+1) && p[k+1] == '*'){

while(strlen(s) != 0 && specific_range(1,k-1,p,s[0])){
if(is_Match(s,substr(k+2,p)))
return true;
else{

s = substr(1,s);

}
}
return is_Match(s,substr(k+2,p));

}
else if(lenp > (k+1) && p[k+1] == '?'){

if(specific_range(1,k-1,p,s[0])){
return is_Match(s,substr(k+2,p)) || is_Match(substr(1,s),substr(k+2,p));
}
else{
return is_Match(s,substr(k+2,p));
}

}

else{
if(specific_range(1,k-1,p,s[0]))
return is_Match(substr(1,s),substr(k+1,p));
else
return false;
}

}

}
else if(p[1] == '?'){
if(strlen(s) != 0 && (p[0] == s[0])){
return is_Match(s,substr(2,p)) || is_Match(substr(1,s),substr(2,p));
}
else{
return is_Match(s,substr(2,p));
}

}
else if(p[1] != '*'){
if (strlen(s) != 0 && (p[0] == s[0]))
return is_Match(substr(1,s), substr(1,p));
else return false;
}

else {
while(strlen(s) != 0 && (p[0] == s[0])){
if(is_Match(s,substr(2,p)))
return true;
else{

s = substr(1,s);

}
}
return is_Match(s,substr(2,p));
}

}


}






/** print the output of each file or dirictory for the coursework.
* @param name the name a file or a directory.
*/
void print_output(const char *name){

printf(SIGN);	
printf("|--- %s\n", name);
}




/** print all files or directories under a certain path or matching the given pattern.
* @param regex_mode used to determine print the matching result or all contents under a path. If regex_mode is true, print all matching result, otherwise print all contents under a path.
* @param regex the pattern to match.
*/
void print_dir_and_file(bool regex_mode, char *regex){


struct dirent de;
struct dirent de_2;
int i = 0;
bool j = true;
bool m = false;

HDIR dir = opendir(PATH, 0);
HDIR dir1 = opendir(PATH, 0);
if(!regex_mode)
readdir(dir1, &de_2);
   while (readdir(dir, &de)) {
   if(regex_mode){
   if(j){
   readdir(dir1, &de_2);
   }
   if(m = is_Match((char *)de.name,regex)){
   j = true;
   
   while(readdir(dir1, &de_2)){
   if(is_Match(de_2.name,regex)){
   j = false;
   break;   
   }
   
   }
   }
   
   }
   else{
   j = !readdir(dir1, &de_2);
   }
   
   
   if(m || !regex_mode){
   int sign_size = SIGN_SIZE;
   int size = SIZE;
   strcat(de.name);
   
   HFILE o_f = open(PATH,0);
   HDIR dir_1 = opendir(PATH, 0);
   
  if(!is_error(o_f)){
   FILE_NUM++;
   print_output(de.name);
   strsplit(size);
   closedir(dir_1);
   close(o_f);
   }
   
   else if(!is_error(dir_1)){
   DIR_NUM++;
  
   print_output(de.name);
   
   if(j){
   strcat_sign("    ");
   }
   else{
   strcat_sign("|   ");
   }
  
   print_dir_and_file(regex_mode, regex);
   strsplit_sign(sign_size);
   strsplit(size);
   close(o_f);
   closedir(dir_1);
   }
   
   else{
  
  print_output(de.name);
 
   strsplit(size);
   closedir(dir_1);
   close(o_f);
   }
  
   
   }
   }
   
   	


closedir(dir1);
closedir(dir);
}





/** print the entire result for coursework.
* @param end_copy_index the end index for copying cmdline to global variable PATH.
* @param cmdline the command line input.
* @param initial_path the initial path for PATH.
* @param regex_mode used to determine print the matching result or all contents under a path. If regex_mode is true, print all matching result, otherwise print all contents under a path.
* @param regex the pattern to match.
* @param modification to indicate whether PATH need to change.
*/
void print_information(int end_copy_index, const char *cmdline, const char *initial_path, bool regex_mode, char *regex, bool modification){

    
   
    PATH =  (char *)initial_path;
    SIZE += strlen(initial_path);
    if(modification){
    SIZE -= strlen(initial_path);
    strcpy(PATH, (char *)cmdline, 0, end_copy_index);
    SIZE += end_copy_index+1;
    }
    SIGN = (char *)"\0";
    SIGN_SIZE = 0;
    printf(".\n");
   
    print_dir_and_file(regex_mode, regex);
    
   
    printf("\n%d directories, %d files\n", DIR_NUM, FILE_NUM);
 
}




/** find the index of first space in the command line.
* @param c the command line.
* @return return index of first space in given command line.
*/
int index_of_space(char *c){

char *b = c;
int index = 0;
while(*b != ' '){
b++;
index++;
}

return index;
}




/** find the number of spaces in the command line.
* @param input the command line.
* @return return the number of spaces in given command line.
*/
int num_of_space(const char *input){
int num = 0;
const char *in =input;
while(*in){
if(*in == ' '){
num++;
}
in++;
}
return num;

}


int main(const char *cmdline)
{
    // TODO: Implement me!
     const char c[] = "/usr\0";
    if (!cmdline || strlen(cmdline) == 0){

    print_information(0,cmdline, c,false,"", false);
    
    }
    else{
    int space_num = num_of_space(cmdline);
    if(space_num == 1){
   
    char *regex = substr(3, (char *)cmdline);
    print_information(0, cmdline, c, true, regex, false);
    
    }
    else if (space_num == 2){
 
    int index = index_of_space((char *)cmdline);
   
    char *regex = substr(index+4, (char *)cmdline);

    print_information(index-1, cmdline, c,true, regex, true);
    }
    else{
    
    print_information(0, cmdline, (char *)cmdline,false, "", false);
    
    }
    }
    
    
    return 0;
    
    
}
