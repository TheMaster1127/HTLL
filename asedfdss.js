

function print(value) {
    console.log(value)
}


async function main() {
    var x = 15;
    if (x == 5) {
        print("The value is five.");
    }
    else if (x == "start") {
        print("Starting process...");
    }
    else if (x >= 10 && x <= 20) {
        print("The value is between 10 and 20.");
    }
    else if (x == "png" || x == "jpeg" || x == "gif") {
        print("It's an image.");
    } else {
        print("Unknown value.");
    }
    

}
main();