
void pal_par(char *text){
for(int i=0; text[i] != '\0';i+=2){
printf("%c%c ", text[i],text[i+1] ? text [i+1] : '_');
}
printf("\n");
};
