const drawGrid = document.getElementById('drawGrid');
const previewImage = document.getElementById('previewImage');
const colorPicker = document.getElementById('colorPicker');
const colorHistory = document.getElementById('colorHistory');
const uploadButton = document.getElementById('uploadButton');
const imageUploader = document.getElementById('imageUploader');
const saveButton = document.getElementById("saveButton");
const usbConnectButton = document.getElementById("usbConnectButton");
const usbSendButton = document.getElementById("usbSendButton");

// Number of LEDS total (must be a square since our LED array is 8x8)
const numLedsPerSide = 8;
const numLedsTotal = numLedsPerSide * numLedsPerSide;

// Commands that exist on the firmware
const COMMANDS = {
    SETPIXEL: 0x1,
    UPDATEPIXEL: 0x2,
    UPDATEPIXELALL: 0x3,
    SETPIXELSDEFAULTCOLOR: 0x4,
};

const recentColors = [];

let gridCells = [];
let previewCells = [];

// Serial port object and writer object to write to serial port
let port = null;
let writer = null;

//-------------- Functions----------------

/* GridInit
   Draw the main grid, mini preview grid, and init the important stuff to draw
 */
function GridInit()
{
    for (let i = 0; i < numLedsTotal; i++)
    {
        // Create the elements to show the "pixels"
        let cell = document.createElement('div');
        let previewCell = document.createElement('div');

        // Assign the style class
        cell.classList.add('cell');
        previewCell.classList.add('previewCell');

        cell.style.backgroundColor = '#000000';
        previewCell.style.backgroundColor = '#000000';

        /* Add an event listener to handle the click function to update the main and mini preview.
           We also update our color history with the new color, and send the USB data to the device
         */
        cell.addEventListener('click', () => {
            const selectedColor = colorPicker.value;
            cell.style.backgroundColor = selectedColor;
            previewCell.style.backgroundColor = selectedColor;
            UpdateColorHistory(selectedColor);
            SendSinglePixelUpdate(i);
        });

        // Add an event listener to color the pixel black (AKA OFF) and send to USB
        cell.addEventListener('contextmenu', (event) => {
            event.preventDefault(); // Stop default right click menu
            cell.style.backgroundColor = '#000000';
            previewCell.style.backgroundColor = '#000000';
            SendSinglePixelUpdate(i);
        });

        // Add the pixel cells to the grid so we can see them
        drawGrid.appendChild(cell);
        previewImage.appendChild(previewCell);
        gridCells.push(cell);
        previewCells.push(previewCell);
    }
}


/* UpdateColorHistory
   Adds a newly selected color to the 8 most recent colors list
   color - RGBA color value of new color
 */
function UpdateColorHistory(color)
{
    if (!recentColors.includes(color))
    {
        recentColors.unshift(color);
        if (recentColors.length > 8)
        {
            recentColors.pop();
        }
    }

    colorHistory.innerHTML = ''; // Need to clear current history first before repopulating
    recentColors.forEach((recentColor) => {
        // Create the HTML elements and set the colors
        let colorBox = document.createElement('div');
        colorBox.classList.add('colorBox');
        colorBox.style.backgroundColor = recentColor;

        // We add an event listener so we can pick a color from the color history box
        colorBox.addEventListener('click', () => {
            colorPicker.value = recentColor;
        });

        colorHistory.appendChild(colorBox);
    });
}

/* SendDataToSerialDevice
   Transmits the serial data to the connected USB device
   data - byte stream of the data
 */
const SendDataToSerialDevice = async (data) => {
    // A stream to output the serial stream must be connected
    if (!writer)
    {
        console.error('No active connection. Please connect first.');
        return;
    }

    try
    {
        let bytes = new Uint8Array(data);
        await writer.write(bytes);
        console.log('Data sent:', bytes);
    }
    catch (error)
    {
        console.error('Error sending data:', error);
    }
};

/* SendPixel
   Send the data of a single pixel
   num - The number of the pixel in the LED display (indexed from 0)
 */
function SendPixel(num)
{
    if(num < numLedsTotal && num >= 0)
    {
        let col = gridCells[num].style.backgroundColor;
        let rgb = col.replace(/[^\d,]/g, '').split(',');
        let r = parseInt(rgb[0], 10);
        let g = parseInt(rgb[1], 10);
        let b = parseInt(rgb[2], 10);

        if(r >= 0 && r < 256 && g >= 0 && g < 256 && b >= 0 && b < 256)
        {
            SendData([COMMANDS.SETPIXEL, num, r, g, b])
        }
        else
        {
            console.error('Invalid RGB value, must be 0-255.');
        }
    }
    else
    {
        console.error('LED number is greater than number of actual LEDs.');
    }
}

/* SendSinglePixelUpdate
   Tell the device to calculate the SPI bitstream for the LED
   and then update the LED display
   num - The number of the pixel in the LED display (indexed from 0)
   r - 8-bit number representing color
   g - 8-bit number representing color
   b - 8-bit number representing color
 */
function SendSinglePixelUpdate(num, r, g, b)
{
    SendPixel(num, r, g, b);
    SendData([COMMANDS.UPDATEPIXEL, num, 0x0, 0x0, 0x0]);
}

/* SendAllPixelUpdate
   Tell the device to calculate the SPI bitstream for all LEDs
   and then update the LED display
 */
function SendAllPixelUpdate()
{

    for(let i = 0; i < numLedsTotal; i++)
    {
        SendPixel(i);
    }
    SendData([COMMANDS.UPDATEPIXELALL, 0x0, 0x0, 0x0, 0x0]);
}

/* SendAllPixelDefaultColor
   Reset the LEDs to default color and tell the device to calculate
   the SPI bitstream for all LEDs and then update the LED display
 */
function SendAllPixelDefaultColor()
{
    SendData([COMMANDS.SETPIXELSDEFAULTCOLOR, 0x0, 0x0, 0x0, 0x0]);
    SendData([COMMANDS.UPDATEPIXELALL, 0x0, 0x0, 0x0, 0x0]);
}

/* SendData
   Transmit a byte stream to the device
   This was broken out into another function to help with debugging
 */
function SendData(dataArray)
{
    SendDataToSerialDevice(dataArray).catch(error => {
        console.error('Error sending: ${data}, Error: ${error}', error);
    });

    //console.log(dataArray);
}

//-------------- Event listeners ----------------

// Event listener to trigger grid initialization when page loaded
document.addEventListener('DOMContentLoaded', function()
{
    GridInit();
});

// Event listener to trigger image upload event listener when file chosen
uploadButton.addEventListener('click', () =>
{
    imageUploader.click();
});

// Event listener for handling when a file is selected
imageUploader.addEventListener('change', (event) =>
{
    // Get file from form event
    let file = event.target.files[0];
    if (file)
    {
        // Start to read the file
        const reader = new FileReader();
        reader.onload = (e) =>
        {
            // Load image into an image object
            let img = new Image();
            img.src = e.target.result;
            img.onload = () =>
            {
                if (img.width !== numLedsPerSide || img.height !== numLedsPerSide)
                {
                    alert('Image must be ' + numLedsPerSide + ' x ' + numLedsPerSide + ' pixels.');
                    return;
                }

                // Draw the image on the grid on a new canvas
                let canvas = document.createElement('canvas');
                canvas.width = numLedsPerSide;
                canvas.height = numLedsPerSide;
                const ctx = canvas.getContext('2d');
                ctx.drawImage(img, 0, 0, numLedsPerSide, numLedsPerSide);
                const imageData = ctx.getImageData(0, 0, numLedsPerSide, numLedsPerSide);

                // Plot the actual pixel RGBA data on the image
                for (let i = 0; i < numLedsTotal; i++)
                {
                    const r = imageData.data[i * 4];
                    const g = imageData.data[i * 4 + 1];
                    const b = imageData.data[i * 4 + 2];
                    const a = imageData.data[i * 4 + 3];
                    const color = `rgba(${r}, ${g}, ${b}, ${a / 255})`;

                    // Update the grid cells and the preview cells
                    gridCells[i].style.backgroundColor = color;
                    previewCells[i].style.backgroundColor = color;
                }
                // Update the USB device
                SendAllPixelUpdate();

            };
        };
        reader.readAsDataURL(file);
    }
});

// Event listener for handling saving the drawn image file
saveButton.addEventListener("click", () =>
{
    const filename = prompt("Enter a filename for your image:", "ledPicture.png");

    if (!filename)
    {
        return;
    }

    // Create a new canvas for image to save to
    const canvas = document.createElement('canvas');
    canvas.width = numLedsPerSide;
    canvas.height = numLedsPerSide;
    const ctx = canvas.getContext('2d');

    // Plot each pixel color on the canvas
    gridCells.forEach((cell, index) => {
        let x = index % numLedsPerSide;
        let y = Math.floor(index / numLedsPerSide);
        ctx.fillStyle = getComputedStyle(cell).backgroundColor;
        ctx.fillRect(x, y, 1, 1);
    });

    /* To download the image, we have to get the URL and then fake a click to bring up
       the proper dialog to save it */
    const imageUrl = canvas.toDataURL("image/png");
    const link = document.createElement('a');
    link.href = imageUrl;
    link.download = filename;
    link.click();
});

// Event listener to handle click to connect to the USB device
usbConnectButton.addEventListener("click", async () =>
{
    // We need to check if webserial is supported for the browser
    if ('serial' in navigator)
    {
        // Yippie!
        console.log("WebSerial supported.");
        try
        {
            // Try to open the requested device for serial comms @ 115200 baud
            port = await navigator.serial.requestPort();
            await port.open({ baudRate: 115200 });
            console.log('Serial port opened');

            // Stream for outputting data
            writer = port.writable.getWriter();

            SendAllPixelDefaultColor();
        }
        catch (error)
        {
            console.error('Serial port unable to be opened. Error:', error);
        }

    }
    else
    {
        alert("Your browser does not support WebSerial!");
    }
});