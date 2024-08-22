String HTML PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>test</title>
  <style>
    html, body {
      width: 100%;
      height:100%;
      background-color: #f0f0f0;
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
    }
    main {
      width:60%;
    }
    .fieldsetgrey {
      background-color: #f0f0f0;
      padding: 10px;
      margin-bottom: 20px;
      border: 3px solid black;
      border-radius: 6px;
    }
    .fieldsetgreen {
      background-color: #e0f7e0;
      padding: 10px;
      margin-bottom: 20px;
      border: 2px solid #4CAF50;
      border-radius: 6px;
    }
    .fieldsetyellow {
      background-color: #fff9e0;
      padding: 10px;
      margin-bottom: 20px;
      border: 2px solid #FFCE00;
      border-radius: 6px;
    }
    .fieldsetred {
      background-color: #f7e0e0;
      border: 2px solid #F70000;
    }
    .fieldsethome {
      border: 3px solid black;
      border-radius: 6px;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
    }
    legend {
      font-weight: bold;
    }
    #uploadForm {
      display: flex;
      flex-direction: row;
      align-items: center;
      justify-content: space-around;
    }
    input[type=file] {
      width: 350px;
      max-width: 100%;
    }
    #uploadForm input {
      margin-left: 10px;
      margin-bottom: 10px;
    }
    #uploadForm p {
      margin-left: 20px;
      font-weight: bold;
      font-size: 1.5em;
    }
    input[type=file] {
      width: 350px;
      max-width: 100%;
    }
    input[type="submit"], input[type=file]::file-selector-button  {
      color: white;
      text-shadow: -1px 0 #008000, 0 1px #008000, 1px 0 #008000, 0 -1px #008000;
      background-color: #4CAF50; /* Couleur de fond personnalisée */
      font-weight: bold;
      margin-right: 20px;
      padding: 10px 10px;
      border: 2px solid #008000;
      border-radius: 8px;
      cursor: pointer;
    }
    input[type="submit"]:hover, input[type=file]::file-selector-button:hover {
      background-color: #008000;
      border: 2px solid #4CAF50;
    }
    progress {
      width: 100%;
      height: 20px;
      border-radius: 6px;
      overflow: hidden;
    }
    progress::-webkit-progress-bar {
      background-color: #f3f3f3;
    }
    progress::-webkit-progress-value {
      background-color: #4CAF50;
    }
    progress::-moz-progress-bar {
      background-color: #4CAF50;
    }
    .downloadButton {
        margin-left: 20px;
        font-weight: bold;
        background-color: #90EE90;
        padding: 5px 5px;
        border: 2px solid #008000;
        border-radius: 8px;
        cursor: pointer;
    }
    .deleteButton {
        margin-left: 20px;
        font-weight: bold;
        background-color: #FFC0CB;
        padding: 5px 5px;
        border: 2px solid #FF0000;
        border-radius: 8px;
        cursor: pointer;
    }
    .formatButton {
        margin : center;
        margin-bottom: 10px;
        font-weight: bold;
        background-color: #FFDE00;
        color: #A52A2A;
        padding: 5px 5px;
        border: 2px solid #FFA500;
        border-radius: 8px;
        cursor: pointer;
    }
    .homeButton {
        margin : center;
        padding: 10px 20px;
        background: linear-gradient(to right, #90EE90, #FFFF82, #FFC0CB);
        color: black;
        border: 2px solid black;
        border-radius: 6px;
        cursor: pointer;
        font-size:16px;
        font-weight: bold;
    }
    .custom-alert {
        display: none;
        position: fixed;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        background-color: #E5E5E5;
        border: 2px solid black;
        box-shadow: 0 2px 10px grey;
        padding: 20px;
        border-radius: 8px;
        z-index: 1000;
        text-align: center;
    }
    .custom-alert p {
        margin: 0;
        font-family: Arial, sans-serif;
        color: black;
        font-size:16px;
        font-weight: bold;
    }
    #alerte {
        color: black;
    }
    #redalerte {
        color: red;
    }
    .overlay {
        display: none;
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background-color: rgba(0, 0, 0, 0.5);
        z-index: 999;
    }
    .custom-alert button {
        margin-top: 10px;
        padding: 10px 20px;
        background: linear-gradient(to right, #90EE90, #FFFF82, #FFC0CB);
        color: black;
        border: 2px solid black;
        border-radius: 6px;
        cursor: pointer;
        font-size:16px;
        font-weight: bold;
    }
    .custom-alert button:hover {
        background: linear-gradient(to right, green, yellow, red);
    }
    #SizeList {
        text-align: center;
        font-size: 16px;
        font-weight: bold;
        margin-bottom: 5px;
        padding: 5px 5px;
    }
    #fileList {
        display: flex;
        flex-direction: column;
        align-items: middle;
        justify-content: space-around;
        margin-bottom: 5px;
        padding: 5px 5px;
        font-size: 16px;
        font-weight: bold;
    }
  </style>
</head>
<body>
  <main>
    <fieldset class="fieldsetgrey">
      <legend>ESP32 FSorOTA</legend>
      <fieldset class="fieldsetgreen">
        <legend>OTA ou Upload d'un fichier dans LittleFS</legend>
        <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data">
          <p><input id="fileInput" type="file" name="file"></p>
          <p><input type="submit" value="Upload"></p>
        </form>
        <progress id="progressBar" value="0" max="100"></progress>
      </fieldset>
      <fieldset class="fieldsetyellow">
        <legend>Espace disponible dans LittleFS</legend>
        <pre id="SizeList"></pre>
      </fieldset>
      <fieldset class="fieldsetred">
        <legend>Fichiers présents dans LittleFS</legend>
        <pre id="fileList"></pre>
      </fieldset>
    </fieldset>
    <fieldset class="fieldsethome">
      <legend>ESP32 Home</legend>
      <button class="homeButton" onclick="window.location.href = '/'">Retour</button>
    </fieldset>
    <div class="overlay" id="overlay"></div>
    <div class="custom-alert" id="customAlert">
      <p id="alerte"></p>
      <p id="redalerte"></p>
      <p><button onclick="closeAlert()">Fermer</button></p>
    </div>
  </main>
  <script>
    const form = document.getElementById('uploadForm');
    const progressBar = document.getElementById('progressBar');
    progressBar.style.display = 'none';
    const SizeList = document.getElementById('SizeList');
    const fileList = document.getElementById('fileList');
    var freeSpace;
    function showAlert(message, redmessage) {
      document.getElementById('customAlert').style.display = 'block';
      document.getElementById('overlay').style.display = 'block';
      document.getElementById('alerte').innerHTML = message;
      document.getElementById('redalerte').innerHTML = redmessage;
    }
    function closeAlert() {
      document.getElementById('customAlert').style.display = 'none';
      document.getElementById('overlay').style.display = 'none';
      const url = '/FSorOTA';
      window.location.href = url;
    }
    function cleanResult() {
      const xhr = new XMLHttpRequest();
      xhr.open('GET', '/clean', true);
      xhr.onload = function() {
        if (xhr.status === 200) {
          document.getElementById('cleanResult').innerHTML = xhr.responseText;
        }
      };
     xhr.send();
    }
    function checkSizeFS() {
      const xhr = new XMLHttpRequest();
      xhr.open('GET', '/rawsize', true);
      xhr.onload = function() {
        if (xhr.status === 200) {
          freeSpace = xhr.responseText;
        }
      };
     xhr.send();
    }
    function loadSizeFS() {
      const xhr = new XMLHttpRequest();
      xhr.open('GET', '/size', true);
      xhr.onload = function() {
        if (xhr.status === 200) {
          const files = xhr.responseText.split('\n').filter(file => file);
          SizeList.innerHTML = '';
          const formatButton = document.createElement('button');
          formatButton.textContent = 'Formatter';
          formatButton.onclick = () => {
              const xhr = new XMLHttpRequest();
              xhr.open('GET', '/clean', true);
              xhr.onload = function() {
                if (xhr.status === 200) {
                  showAlert('', xhr.responseText);
                }
              };
             xhr.send();
         };
          formatButton.classList.add('formatButton');
          SizeList.appendChild(formatButton);
          files.forEach(file => {
            const fileDiv = document.createElement('div');
            fileDiv.textContent = file;
            fileDiv.classList.add('fileList');
            SizeList.appendChild(fileDiv);
          });
        }
      };
      xhr.send();
    }
    function loadFileList() {
      const xhr = new XMLHttpRequest();
      xhr.open('GET', '/list', true);
      xhr.onload = function() {
        if (xhr.status === 200) {
          const files = xhr.responseText.split('\n').filter(file => file);
          fileList.innerHTML = '';
          files.forEach(file => {
            const fileDiv = document.createElement('div');
            fileDiv.textContent = file;
            const downloadButton = document.createElement('button');
            downloadButton.textContent = 'Télécharger';
            downloadButton.onclick = () => {
              const url = `/download?file=${encodeURIComponent(file)}`;
              window.location.href = url;
            };
            downloadButton.classList.add('downloadButton');
            fileDiv.appendChild(downloadButton);
            const deleteButton = document.createElement('button');
            deleteButton.textContent = 'Effacer';
            deleteButton.onclick = () => {
              const url = `/effacer?file=${encodeURIComponent(file)}`;
              const xhr = new XMLHttpRequest();
              xhr.open('GET', url, true);
              xhr.onload = function() {
                if (xhr.status === 200) {
                  showAlert('', xhr.responseText);
                }
              };
              xhr.send();
            };
            deleteButton.classList.add('deleteButton');
            fileDiv.appendChild(deleteButton);
            fileList.appendChild(fileDiv);
          });
        }
      };
      xhr.send();
    }
    form.addEventListener('submit', function(event) {
      event.preventDefault();
      var file = document.getElementById('fileInput').files[0];
      if (file) {
        if (file.size > freeSpace) {
          showAlert('Taille du fichier : ' + file.size + ' octets.\n' + 'Espace disponible : ' + freeSpace + ' octets.', 'Espace insuffisant');
          return;
        }
        progressBar.style.display = 'block';
      } else {
        showAlert('', 'Aucun fichier sélectionné');
        return;
      }
      const xhr = new XMLHttpRequest();
      const formData = new FormData(form);
      xhr.upload.addEventListener('progress', function(event) {
        if (event.lengthComputable) {
          const percentComplete = (event.loaded / event.total) * 100;
          progressBar.value = percentComplete;
        }
      });
      xhr.onload = function() {
        if (xhr.status === 200) {
          showAlert('', xhr.responseText);
        } else {
          showAlert('${file}', 'File upload failed');
        }
      };
      xhr.addEventListener('load', function() {
        loadFileList();
      });
      xhr.open('POST', '/upload');
      xhr.send(formData);
    });
    window.onload = function(){
      checkSizeFS();
      loadSizeFS();
      loadFileList();
    }
  </script>
</body>
</html>
)=====";
