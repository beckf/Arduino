int recycleButton = 8;
int trashButton = 7;

char* questions[] = {
  "bubble wrap",
  "paper",
  "can"
};

char* answers[] = {
  "trash",
  "recycle",
  "recycle"
};

int questionCount = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(recycleButton, INPUT_PULLUP);
  pinMode(trashButton, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int questionNum = 0; questionNum < questionCount; questionNum++) {
    // Get the correct answer from the answer array
    char* answer = answers[questionNum];
    
    Serial.println(questions[questionNum]);
    // wait here unitil a button is pressed.  Set state to HIGH/LOW.  Whichever is off.
    while((digitalRead(trashButton) == LOW) && (digitalRead(recycleButton) == LOW)) {
      // We will wait here forever until we get an answer.
      Serial.print("waiting to see if they pick ");
      Serial.println(answer);
    }

    // what to do if they press trash
    if (digitalRead(trashButton) == HIGH) {
      // Trash was selected
      if (answer == "trash") {
        Serial.println("Correct");
        delay(3000);
      } else { 
        Serial.println("Wrong");
        delay(3000);
      }
    }

    //what to do if they press recycle
    if (digitalRead(recycleButton) == HIGH) {
      // Trash was selected
      if (answer == "recycle") {
        Serial.println("Correct");
        delay(3000);
      } else { 
        Serial.println("Wrong");
        delay(3000);
      }
    }
  }
}
