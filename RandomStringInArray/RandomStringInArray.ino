/* 
 *  This example shows how to print random phrases to the Serial Monitor
 *  using pointers.
 *  
 */
 
char* phrases[]={"Phrase Text 1", "Phrase Text 2", "Phrase Text 3"};

void setup() {
  Serial.begin(9600);
}

void loop() {
  int phrasesArraySize = sizeof(phrases) / sizeof (char*);
  int randomPhrase = random(0, phrasesArraySize);
  Serial.println(phrases[randomPhrase]);
}
