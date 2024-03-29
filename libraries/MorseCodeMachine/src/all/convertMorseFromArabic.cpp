#include "_countUtf8Bytes.h"
#include "_enFromArabicMorse.h"
#include "_enToMorseElements.h"
#include "_skipDiacritic.h"
#include "_writeUnicodeFFFD.h"
#include "convertMorseFromArabic.h"
#include <string.h>

using namespace b1ccef0c36f5537eb1a608b20bb25eb318bbf795;

int convertMorseFromArabic(const char arabicMessage[], char englishBuffer[], size_t englishBufferSize)
{
    if (!(arabicMessage && englishBuffer && (englishBufferSize >= 1)))
    {
        return -1;
    }

    char *destination = englishBuffer;
    char *destinationEnd = englishBuffer + englishBufferSize;
    const char *source = arabicMessage;
    const char *sourceReset = arabicMessage;

    while (*source)
    {
        //=====================================================================
        //Translate Arabic characters.
        //=====================================================================

        if (_enFromArabicMorse(&source, &destination, destinationEnd) == 1)
        {
            //Not enough space to translate.
            *destination = '\x00';
            return 1;
        }
        else if (sourceReset != source)
        {
            //Translation successful.
            sourceReset = source;
            continue;
        }
        else
        {
            //Reset.
            source = sourceReset;
        }

        //=====================================================================
        //Translate English letters, digits, and symbols defined by Morse code.
        //=====================================================================

        if (_enToMorseElements(&source))
        {
            size_t countBytes = source - sourceReset;
            if (destinationEnd <= destination + countBytes)
            {
                //Not enough space to translate.
                *destination = '\x00';
                return 1;
            }
            memcpy(destination, sourceReset, countBytes);
            destination += countBytes;

            //Translation successful.
            sourceReset = source;
            continue;
        }
        else
        {
            //Reset.
            source = sourceReset;
        }

        //=====================================================================
        //Unknown character.
        //=====================================================================

        unsigned char countUtf8Bytes = _countUtf8Bytes(source);

        //Go to next UTF-8 character.
        source += (countUtf8Bytes > 0) ? countUtf8Bytes : 1;

        //Skip diacritics only if valid UTF-8 character found.
        if (countUtf8Bytes > 0)
        {
            while (_skipDiacritic(&source))
                ;
        }

        //Translate unknown character to U+FFFD.
        if (!_writeUnicodeFFFD(&destination, destinationEnd))
        {
            //Not enough space to translate.
            *destination = '\x00';
            return 1;
        }
        else
        {
            //Translation successful.
            sourceReset = source;
            continue;
        }
    }

    *destination = '\x00';
    return 0;
}

