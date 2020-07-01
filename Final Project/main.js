let arrSize = 50
let mainArr = getNewArr();

let canvas = document.getElementById('mainCanvas');
let c = canvas.getContext('2d');

// Make canvas as large as the user's screen
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;
// c.font = "20px Arial";
// c.textAlign = "center";

// Create rectangles with colour and length based the number size in array
for (let i = 0; i < arrSize; i++) {
    c.fillStyle = getHSL(mainArr[i]);

    rect = getPos(i, mainArr[i])

    c.fillRect(rect.x, canvas.height - 20, 15, rect.height)
    // c.fillText(mainArr[i], 100 + i * 26, canvas.height / 2 + 7);
    // c.fillRect(90 + i * 20, canvas.height / 2 + 10, 18, mainArr[i] * 6)
}


function shuffle(array) {
    let currentIndex = array.length, temporaryValue, randomIndex;
  
    // While there remain elements to shuffle...
    while (0 !== currentIndex) {
  
        // Pick a remaining element...
        randomIndex = Math.floor(Math.random() * currentIndex);
        currentIndex -= 1;
    
        // And swap it with the current element.
        temporaryValue = array[currentIndex];
        array[currentIndex] = array[randomIndex];
        array[randomIndex] = temporaryValue;
    }
    return array;
  }

// Generate new random array
function getNewArr() {
    let array = [];
    for(let i = 1; i <= arrSize; i++){
        array.push(i);
    }
    shuffle(array);
    return array; 
}

// Find HSL Hue value based on number size in array
function getHSL(i) {
    if (i === 0 || i === arrSize) {
        return "hsl(0, 100%, 50%)";
    }
    else {
        let hue = 360 / arrSize * i;
        return "hsl(" + hue + ", 100%, 50%)";
    }
}

// Find the position of where each rectangle should start
function getPos(i, num) {
    let rectangle = {
        x: (canvas.width - 40) / arrSize + i * (canvas.width - 40) / arrSize,
        width: (canvas.width - 40) / arrSize - 4,
        widthMargin: 4,
        height: ((canvas.height - 40) / arrSize) * -num
    };
    return rectangle
}

console.log(rect);