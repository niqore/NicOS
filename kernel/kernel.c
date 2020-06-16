/*
Permet d'éviter au kernel à sauter en kernel.c:0x00
En effet, la place de ce code dépend du linker, il peut se trouver aussi bien au début qu'à la fin.
En mettant cette fonction, comme l'ordre est gardé, on s'assure de passer dans main()
*/
void dummy_test_entrypoint() {
}

void main() {
	char* video_memory = (char*) 0xb8000;
    *video_memory = 'X'; /* On écrit un X en première position */
}