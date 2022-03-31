#include <MorseCodeMachine.h>

//Create a character array that can store 100 characters.
//This will be used to hold the English translation of Greek characters.
char englishArray[100];

void setup()
{
    //You are going to use the built-in LED in the Arduino to send Morse code.
    pinMode(LED_BUILTIN, OUTPUT);

    //Use the Serial monitor to look at the Greek and English messages.
    Serial.begin(9600);
}

void loop()
{
    //You are going to send "γεια World!" as Morse code.
    //=========================================================================
    //Warning: Do not use accent marks on any Greek character.
    //         Greek Morse code does not use characters with accent marks.
    //
    //         See the following for more details about Greek Morse code:
    //         Morse code for non-Latin alphabets - Wikipedia
    //         https://en.wikipedia.org/wiki/Morse_code_for_non-Latin_alphabets
    //
    //         Any unrecognized characters will be replaced with the Unicode
    //         replacement character (U+FFFD).
    //=========================================================================
    const char greekMessage[] = "γεια World!";

    //Convert Greek into its English equivalent with convertMorseFromGreek.
    //The 100 at the end tells convertMorseFromGreek that englishArray can only
    //hold 100 characters.
    int errorCode = convertMorseFromGreek(greekMessage, englishArray, 100);

    //Check if there were any errors from convertMorseFromGreek.
    //convertMorseFromGreek returns 0 if there were no errors.
    if (errorCode == 0)
    {
        //Send the greek and english messages to the Serial Monitor if you want
        //to see the Greek and the English translation used for the Morse code.
        Serial.println(greekMessage);
        Serial.println(englishArray);
        
        //Since there were no errors, send Morse code using englishArray.
        sendMorse(englishArray, ledDelay, ledDot, ledDash);
    }
    else
    {
        //Send an error message to the Serial monitor.
        //The most likely reason an error happens is that the size of
        //englishArray is not big enough to contain the translation.
        Serial.println("There was an error converting this to English:");
        Serial.println(greekMessage);
    }
}

//Create a delay function and give it to the sendMorse function so that the
//sendMorse function can space out the letters and the words properly.
//You can give this function any name you want.
void ledDelay()
{
    //Wait for a small amount of time.
    delay(200);
}

//Create a dot function and give it to the sendMorse function so that the
//sendMorse function can create dots.
//You can give this function any name you want.
void ledDot()
{
    //Turn on the LED for a small amount of time, then turn off.
    digitalWrite(LED_BUILTIN, HIGH);
    ledDelay();
    digitalWrite(LED_BUILTIN, LOW);
}

//Create a dash function and give it to the sendMorse function so that the
//sendMorse function can create dashes.
//You can give this function any name you want.
void ledDash()
{
    //Turn on the LED three times as long as the dot, then turn off.
    digitalWrite(LED_BUILTIN, HIGH);
    ledDelay();
    ledDelay();
    ledDelay();
    digitalWrite(LED_BUILTIN, LOW);
}

