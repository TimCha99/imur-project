const fwbt = document.getElementById('fw');
const bkbt = document.getElementById('bk');
const lfbt = document.getElementById('lf');
const rtbt = document.getElementById('rt');
const apt = document.getElementById('apt');

const slider = document.getElementById('customRange3');
const valueDisplay = document.getElementById('rangeValue');

valueDisplay.textContent = slider.value;

slider.addEventListener('input', () => {
    valueDisplay.textContent = slider.value;
});



function sendCommand(state) {
    fetch('/button', {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: new URLSearchParams({ state })
    });
}


fwbt.addEventListener('mousedown', () => sendCommand('f'));
bkbt.addEventListener('mousedown', () => sendCommand('b'));
lfbt.addEventListener('mousedown', () => sendCommand('l'));
rtbt.addEventListener('mousedown', () => sendCommand('r'));
apt.addEventListener('mousedown', () => sendCommand('a' + slider.value));

fwbt.addEventListener('touchstart', () => sendCommand('f'));
bkbt.addEventListener('touchstart', () => sendCommand('b'));
lfbt.addEventListener('touchstart', () => sendCommand('l'));
rtbt.addEventListener('touchstart', () => sendCommand('r'));
apt.addEventListener('touchstart', () => sendCommand('a' + slider.value));


[fwbt, bkbt, lfbt, rtbt].forEach(btn => {
    btn.addEventListener('mouseup', () => sendCommand('s'));
});

[fwbt, bkbt, lfbt, rtbt].forEach(btn => {
    btn.addEventListener('touchend', () => sendCommand('s'));
});

document.addEventListener('keydown', (event) => {
    if (event.repeat) return; 
    if (event.key === 'w') sendCommand('f');  
    if (event.key === 's') sendCommand('b'); 
    if (event.key === 'a') sendCommand('l'); 
    if (event.key === 'd') sendCommand('r');
});

document.addEventListener('keyup', (event) => {
    if (['w', 'a', 's', 'd'].includes(event.key)) {
        sendCommand('s'); 
    }
});



