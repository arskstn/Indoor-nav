// DOM Elements
const mapImg = document.getElementById('map-image');
const gridCanvas = document.getElementById('map-grid');
const player = document.getElementById('player');
const logContent = document.getElementById('log-content');
const logCount = document.getElementById('log-count');

// Grid cell dimensions
let cellWidth, cellHeight;

// Socket connection
const socket = io();

// Initialize when image loads
mapImg.onload = function() {
    initGrid();
    setupSocket();
    updatePlayerPosition({x: 0, y: 0});
};

function initGrid() {
    // Set canvas to match image exactly
    gridCanvas.width = mapImg.naturalWidth;
    gridCanvas.height = mapImg.naturalHeight;
    
    // Calculate cell dimensions
    cellWidth = gridCanvas.width / GRID_COLS;
    cellHeight = gridCanvas.height / GRID_ROWS;
    
    drawGrid();
}

function drawGrid() {
    const ctx = gridCanvas.getContext('2d');
    
    // Clear previous grid
    ctx.clearRect(0, 0, gridCanvas.width, gridCanvas.height);
    
    // Grid style - THICK RED LINES
    ctx.strokeStyle = "rgba(255, 0, 0, 0.8)";
    ctx.lineWidth = 3;
    
    // Draw vertical lines (X-axis)
    for (let x = 0; x <= GRID_COLS; x++) {
        ctx.beginPath();
        ctx.moveTo(x * cellWidth, 0);
        ctx.lineTo(x * cellWidth, gridCanvas.height);
        ctx.stroke();
    }
    
    // Draw horizontal lines (Y-axis)
    for (let y = 0; y <= GRID_ROWS; y++) {
        ctx.beginPath();
        ctx.moveTo(0, gridCanvas.height - y * cellHeight);
        ctx.lineTo(gridCanvas.width, gridCanvas.height - y * cellHeight);
        ctx.stroke();
    }
}

function updatePlayerPosition(pos) {
    // Convert grid coordinates to pixel positions
    const pixelX = pos.x * cellWidth;
    const pixelY = gridCanvas.height - (pos.y + 1) * cellHeight;
    
    // Position the player (as percentage of container)
    const container = document.getElementById('map-container');
    player.style.left = `${(pixelX / container.offsetWidth) * 100}%`;
    player.style.top = `${(pixelY / container.offsetHeight) * 100}%`;
    
    // Size player to match grid cell
    player.style.width = `${(cellWidth / container.offsetWidth) * 100}%`;
    player.style.height = `${(cellHeight / container.offsetHeight) * 100}%`;
}

function setupSocket() {
    // Position updates
    socket.on('position_update', (pos) => {
        updatePlayerPosition(pos);
    });
    
    // Initial logs
    socket.on('initial_logs', (data) => {
        data.logs.forEach(log => addLogEntry(log));
    });
    
    // New log entries
    socket.on('log_update', (data) => {
        addLogEntry(data.log);
    });
}

function addLogEntry(message) {
    const entry = document.createElement('div');
    entry.textContent = message;
    logContent.prepend(entry);
    logCount.textContent = parseInt(logCount.textContent) + 1;
    
    // Keep only last 20 entries
    if (logContent.children.length > 20) {
        logContent.removeChild(logContent.lastChild);
        logCount.textContent = 20;
    }
}

// Handle window resize
window.addEventListener('resize', () => {
    if (mapImg.complete) {
        drawGrid();
        socket.emit('request_position');
    }
});
