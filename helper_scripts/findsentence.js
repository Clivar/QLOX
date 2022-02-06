const LETTERS =
    "HETDAGNBPLUISZEG" +
    "RALKISTEENZESELF" +
    "DRIEVIERVIJFTWEE" +
    "ZEVENACHTNEGENEN" +
    "DERTIENVEERTIENY" +
    "TWAALFHKWARTSUUR" +
    "TWINTIGMOVERVOOR" +
    "HALFXEENTWEEDRIE" +
    "VIERVIJFZESZEVEN" +
    "ACHTNEGENTIENELF" +
    "TWAALFDERTIGDMEI" +
    "FEBRUARIJUNIJULI" +
    "XAPRILGTAUGUSTUS" +
    "SEPTEMBERJANUARI" +
    "TRMAARTWOKTOBERK" +
    "NOVEMBERDECEMBER";
const COLUMN_DIMENSION = 16;

let word = "HET IS NEGEN UUR";
let hitIndex = -1;
// Keep the index of the end of the last successful found word
let nextWordStartIndex = 0;

// The LEDs are mounted from bottom to top, left to right then right to left etc
function convertToLedIndex(index, length) {
    const invertedIndex = LETTERS.length - index;
    const rowFromBottom = Math.floor((invertedIndex - 1) / COLUMN_DIMENSION);
    const blocksFromBottom = rowFromBottom * COLUMN_DIMENSION;
    const remainder = invertedIndex % COLUMN_DIMENSION;
    // Count from left
    if (rowFromBottom % 2 == 0) {
        if (remainder > 0) {
            return blocksFromBottom + (COLUMN_DIMENSION - remainder);
        } else {
            return blocksFromBottom;
        }
    }
    // Count from right 
    else {
        if (remainder > 0) {
            return blocksFromBottom + remainder - length;
        } else {
            return blocksFromBottom + COLUMN_DIMENSION - length;
        }
    }
}

// The result consists of every successful found word index + length
let result = [];
for (let i = 0; i < LETTERS.length; i++) {
    // End the loop early if we have no more words to look for
    if (nextWordStartIndex >= word.length - 1) {
        break;
    }

    for (let j = nextWordStartIndex; j < word.length; j++) {
        const hitLength = i - hitIndex + 1;
        const wordLength = j - nextWordStartIndex + 1;
        // We reached the end of the word if we are at the end of our words or we encounter a space in the next index
        const endOfWord = j == word.length - 1 || word[j + 1] == ' ';
        if (LETTERS[i] == word[j] && hitIndex < 0) {
            console.log(`Found potential start`);
            hitIndex = i;
        }
        // Keep holding the hitindex until we finish our word
        if (endOfWord && LETTERS[i] == word[j] && hitIndex >= 0 && hitLength == wordLength) {
            console.log(`Found final letter (${LETTERS[i]}) on index ${i}`);
            result.push([convertToLedIndex(hitIndex, hitLength), hitLength]);
            // Move to the next word by skipping a potential space
            nextWordStartIndex = j + 2;
            // We can also move up in letter indexes since words should never be right next to eachother.
            i = i + 1;
            // Lets start looking for our next word by resetting the hitIndex
            hitIndex = -1;
            break;
            // No hit? Reset it and start over!
        } else if (LETTERS[i] != word[j]) {
            console.log(`Couldn't find ${word[j]}. Encountered ${LETTERS[i]}`);
            hitIndex = -1;
            // The non matched letter could be a potential start
            if (LETTERS[i] == word[nextWordStartIndex]) {
                // Rewind so it gets picked up
                i = i - 1;
            }
            break;
        }

        console.log(`Matched (${LETTERS[i]}) on index ${i}`);
        // Move to the next letter!
        i = i + 1;
    }
}

console.log(result);
