#include "InternetButton/InternetButton.h"

InternetButton b = InternetButton();

// Exposed function to set the number of failed jobs.
int setFailed(String command);
// Failed jobs count
int failedCount;
int unstableCount;

void setup() {
    // Tell b to get everything ready to go
    // Use b.begin(1); if you have the original SparkButton, which does not have a buzzer or a plastic enclosure
    // to use, just add a '1' between the parentheses in the code below.
    b.begin();
    for (int i=1; i < 12; i++) {
        b.ledOn(i, 0, 100, 0);
        delay(10);
    }
    Particle.function("failed", setFailed);
}

void loop() {

}

// Play this when the everything is green!
void zeldaChestSong() {
    // Too long :)
    /* b.playSong("G4,4,A4,4,B4,4,C#5,4");
    b.playSong("G4,4,A4,4,B4,4,C#5,4");

    b.playSong("G#4,4,A#4,4,C5,4,D5,4");
    b.playSong("G#4,4,A#4,4,C5,4,D5,4");

    b.playSong("A4,4,B4,4,C#5,4,D#5,4");
    b.playSong("A4,5,B4,5,C#5,5,D#5,5"); */

    b.playSong("A#4,5,C5,5,D5,5,E5,5");
    b.playSong("A#4,5,C5,5,D5,5,E5,5");

    b.playSong("B4,6,C#5,6,D#5,6,E#5,6");

    b.playSong("C5,6,D5,6,E5,6,F#5,6");

    b.playSong("C#5,6,D#5,6,F5,6,G5,6");

    b.playSong("D5,6,E5,6,F#5,6,G#5,6");

    b.playSong("R1,2");

    b.playSong("A5,3,A#5,3,B5,3,C6,1");
}

// Broken tests are the path to the dark side.
// Broken tests lead to bugs. Bugs lead to regressions. Regressions lead to suffering.
void imperialMarch() {
    b.playSong("G4,3,R1,4,G4,3,R1,4,G4,3,R1,4");
    b.playSong("D#4,3,R1,8,A#5,4,G4,2,R1,4");
    b.playSong("D#4,3,R1,8,A#5,4,G4,1");
}

int setFailed(String command) {
    int commaIdx = command.indexOf(',');

    int newFailedCount = command.substring(0,commaIdx).toInt();
    int newUnstableCount = command.substring(commaIdx + 1).toInt();
    int newTotalErrors = (newFailedCount + newUnstableCount);
    int totalErrors = failedCount + unstableCount;
    bool becameGreen = totalErrors > 0 && newTotalErrors == 0;
    bool becameRed = totalErrors < newTotalErrors;

    failedCount = newFailedCount;
    unstableCount = newUnstableCount;

    if (newTotalErrors == 0) {
        if (becameGreen) {
            // Victory! All Green
            zeldaChestSong();
            b.rainbow(10);
        }
        b.allLedsOff();
        for (int i=1; i < 12; i++) {
            b.ledOn(i, 0, 100, 0);
            delay(10);
        }
    } else {
        if (becameRed) {
            imperialMarch();
        }
        b.allLedsOff();

        for (int i=1; i <= failedCount; i++) {
            b.ledOn(i, 100, 0, 0);
            delay(10);
        }

        for (int i=failedCount + 1; i <= failedCount+unstableCount; i++) {
            b.ledOn(i, 100, 100, 0);
            delay(10);
        }
    }
}
