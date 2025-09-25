const fwbt = document.getElementById('fw');
const bkbt = document.getElementById('bk');
const lfbt = document.getElementById('lf');
const rtbt = document.getElementById('rt');
const apt = document.getElementById('apt');
const svll = document.getElementById('ll');
const svrr = document.getElementById('rr');
const upbt = document.getElementById('up');
const dwbt = document.getElementById('dw');
const qbt = document.getElementById('qq');
const firebt = document.getElementById('fire');
const check1 = document.getElementById('ledcheck');

const slider = document.getElementById('customRange3');
const valueDisplay = document.getElementById('rangeValue');

valueDisplay.textContent = slider.value;

slider.addEventListener('input', () => {
    valueDisplay.textContent = slider.value;
});

check1.addEventListener('change', () => {
    if(check1.checked){
        sendCommand('Y');
    }else{
        sendCommand('N');
    }
});


function sendCommand(state) {
    fetch('/button', {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: new URLSearchParams({ state })
    });
}

// dc motor (forward, backward, left, right, speed set-up)
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

// servo motor(right, left, up, down)
qbt.addEventListener('click', ()=> sendCommand('q'));
firebt.addEventListener('click', ()=> sendCommand('G'));

svll.addEventListener('mousedown', () => sendCommand('x'));
svrr.addEventListener('mousedown', () => sendCommand('y'));
upbt.addEventListener('mousedown', () => sendCommand('u'));
dwbt.addEventListener('mousedown', () => sendCommand('d'));

svll.addEventListener('touchstart', () => sendCommand('x'));
svrr.addEventListener('touchstart', () => sendCommand('y'));
upbt.addEventListener('touchstart', () => sendCommand('u'));
dwbt.addEventListener('touchstart', () => sendCommand('d'));


[svll, svrr].forEach(btn => {
    btn.addEventListener('mouseup', () => sendCommand('o'));
});

[upbt, dwbt].forEach(btn => {
    btn.addEventListener('mouseup', () => sendCommand('O'));
});

[svll, svrr].forEach(btn => {
    btn.addEventListener('touchend', () => sendCommand('o'));
});

[upbt, dwbt].forEach(btn => {
    btn.addEventListener('touchend', () => sendCommand('O'));
});

document.addEventListener('keydown', (event) => {
    if (event.repeat) return; 
    if (event.key === 'ArrowLeft') sendCommand('x');  
    if (event.key === 'ArrowRight') sendCommand('y'); 
});

document.addEventListener('keyup', (event) => {
    if (['ArrowLeft', 'ArrowRight'].includes(event.key)) {
        sendCommand('o'); 
    }
});

document.addEventListener('keydown', (event) => {
    if (event.repeat) return; 
    if (event.key === 'ArrowUp') sendCommand('u');  
    if (event.key === 'ArrowDown') sendCommand('d'); 
});

document.addEventListener('keyup', (event) => {
    if (['ArrowUp', 'ArrowDown'].includes(event.key)) {
        sendCommand('O'); 
    }
});