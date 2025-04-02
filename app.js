document.addEventListener('DOMContentLoaded', () => {
    console.log("JavaScript loaded");

    if (typeof THREE === 'undefined') {
        console.error("Three.js not loaded");
        return;
    }
    console.log("Three.js loaded");

    if (typeof THREE.OrbitControls === 'undefined') {
        console.error("OrbitControls not loaded or incompatible with Three.js version");
        return;
    }
    console.log("OrbitControls loaded");

    const scene = new THREE.Scene();
    const camera = new THREE.PerspectiveCamera(75, window.innerWidth * 0.75 / (window.innerHeight - 150), 0.1, 1000);
    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth * 0.75, window.innerHeight - 150);
    const container = document.getElementById('scene-container');
    if (!container) {
        console.error("Container #scene-container not found");
        return;
    }
    container.appendChild(renderer.domElement);
    console.log("Renderer initialized");

    const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
    scene.add(ambientLight);
    const directionalLight = new THREE.DirectionalLight(0xffffff, 0.5);
    directionalLight.position.set(5, 5, 5);
    scene.add(directionalLight);

    const controls = new THREE.OrbitControls(camera, renderer.domElement);
    if (!controls) {
        console.error("Failed to initialize OrbitControls");
        return;
    }
    controls.target.set(2.5, 1.5, 2.5);
    controls.update();
    console.log("OrbitControls initialized");

    const roomGeometry = new THREE.BoxGeometry(5, 3, 5);
    const roomMaterial = new THREE.MeshBasicMaterial({ color: 0xcccccc, wireframe: true });
    const room = new THREE.Mesh(roomGeometry, roomMaterial);
    room.position.set(2.5, 1.5, 2.5);
    scene.add(room);

    function createAxis(start, end, color, label, labelPos) {
        const geometry = new THREE.BufferGeometry().setFromPoints([start, end]);
        const material = new THREE.LineBasicMaterial({ color: color });
        const axis = new THREE.Line(geometry, material);
        scene.add(axis);

        const arrow = new THREE.ArrowHelper(end.clone().sub(start).normalize(), end, 0.2, color);
        scene.add(arrow);

        const canvas = document.createElement('canvas');
        canvas.width = 64;
        canvas.height = 32;
        const context = canvas.getContext('2d');
        context.font = "20px Arial";
        context.fillStyle = color;
        context.fillText(label, 10, 25);
        const texture = new THREE.CanvasTexture(canvas);
        const spriteMaterial = new THREE.SpriteMaterial({ map: texture });
        const sprite = new THREE.Sprite(spriteMaterial);
        sprite.position.copy(labelPos);
        sprite.scale.set(0.5, 0.25, 1);
        scene.add(sprite);
    }

    createAxis(new THREE.Vector3(0, 0, 2.5), new THREE.Vector3(5, 0, 2.5), 0xff0000, "X", new THREE.Vector3(5.2, 0, 2.5));
    createAxis(new THREE.Vector3(2.5, 0, 2.5), new THREE.Vector3(2.5, 3, 2.5), 0x00ff00, "Y", new THREE.Vector3(2.5, 3.2, 2.5));
    createAxis(new THREE.Vector3(2.5, 0, 0), new THREE.Vector3(2.5, 0, 5), 0x0000ff, "Z", new THREE.Vector3(2.5, 0, 5.2));

    const beaconGeometry = new THREE.SphereGeometry(0.1, 32, 32);
    const beaconMaterial = new THREE.MeshBasicMaterial({ color: 0xff5555 });
    const beacons = [];
    beaconsData.forEach((b, i) => {
        const beacon = new THREE.Mesh(beaconGeometry, beaconMaterial.clone());
        beacon.position.set(b.x, b.z, b.y);
        scene.add(beacon);
        beacons.push(beacon);
    });

    // Анимация мигания маяков
    let isRed = true;
    setInterval(() => {
        isRed = !isRed;
        const newColor = isRed ? 0xff5555 : 0x00ff00;
        beacons.forEach(beacon => {
            beacon.material.color.setHex(newColor);
        });
    }, 1000);

    const clientGeometry = new THREE.SphereGeometry(0.1, 32, 32);
    const clientMaterial = new THREE.MeshBasicMaterial({ color: 0x55aaff });
    const client = new THREE.Mesh(clientGeometry, clientMaterial);
    scene.add(client);

    camera.position.set(7, 5, 7);
    camera.lookAt(2.5, 1.5, 2.5);

    function animate() {
        requestAnimationFrame(animate);
        controls.update();
        renderer.render(scene, camera);
    }
    animate();

    const socket = io.connect('http://' + document.domain + ':' + location.port);
    socket.on('connect', () => {
        console.log("WebSocket connected");
        const logContent = document.getElementById('log-content');
        logContent.innerHTML += "WebSocket connected<br>";
    });
    socket.on('update', (data) => {
        client.position.set(data.client.x, data.client.z, data.client.y);
        console.log("Client position updated:", data.client);
        data.beacons.forEach(beacon => {
            const rssiElement = document.getElementById(`rssi-${beacon.id}`);
            const voltageElement = document.getElementById(`voltage-${beacon.id}`);
            const batteryLevelElement = document.getElementById(`battery-level-${beacon.id}`);
            const batteryTextElement = document.getElementById(`battery-text-${beacon.id}`);
            if (rssiElement) {
                rssiElement.textContent = beacon.rssi !== null ? beacon.rssi : "N/A";
            }
            if (voltageElement) {
                voltageElement.textContent = beacon.voltage !== null ? beacon.voltage.toFixed(2) : "N/A";
            }
            if (batteryLevelElement && batteryTextElement) {
                batteryLevelElement.style.width = `${beacon.battery}%`;
                batteryTextElement.textContent = `${beacon.battery}%`;
            }
            if (beacon.rssi !== null || beacon.voltage !== null || beacon.battery !== null) {
                const logContent = document.getElementById('log-content');
                const packetLog = `ПАКЕТ ОТ маяка номер ${beacon.id}: battery=${beacon.battery}%, wifi_rssi=${beacon.rssi !== null ? beacon.rssi : "N/A"}, voltage=${beacon.voltage !== null ? beacon.voltage.toFixed(2) : "N/A"}V`;
                logContent.innerHTML += packetLog + '<br>';
                logContent.scrollTop = logContent.scrollHeight;
            }
        });
        const logContent = document.getElementById('log-content');
        logContent.innerHTML = data.logs.join('<br>');
        logContent.scrollTop = logContent.scrollHeight;
    });

    window.addEventListener('resize', () => {
        camera.aspect = window.innerWidth * 0.75 / (window.innerHeight - 150);
        camera.updateProjectionMatrix();
        renderer.setSize(window.innerWidth * 0.75, window.innerHeight - 150);
    });
});