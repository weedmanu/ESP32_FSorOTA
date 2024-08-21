String HTML PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>test</title>
  <style>
	body {
	  background-color: #f0f0f0;
	  font-family: Arial, sans-serif;
	  display: flex;
	  flex-direction: column;
	  justify-content: center;
	}
	.main {
	  width:50%;
	  margin:auto;
	}
	fieldset {
	  border: 2px solid #ccc;
	  border-radius: 5px;
	  padding: 10px;
	  margin-bottom: 20px;
	}
	fieldset.green {
	  background-color: #e0f7e0;
	}
	fieldset.yellow {
	  background-color: #fff9e0;
	}
	fieldset.red {
	  background-color: #f7e0e0;
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
	  margin-left: 10px;
	}
	input[type="submit"] {
	  background-color: #4CAF50;
	  color: white;
	  padding: 10px 20px;
	  border: none;
	  border-radius: 5px;
	  cursor: pointer;
	}
	input[type="submit"]:hover {
	  background-color: #45a049;
	}
	progress {
	  width: 100%;
	  height: 20px;
	  border-radius: 5px;
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
	.filelist {
		font-size: 1.2 em;
		font-style: bold;
		margin-bottom: 5px;
		padding: 5px 5px;
	}
	.deleteButton {
		margin-left: 10px;
		font-style: bold;
	}
	.cleanButton {
		margin: 0;
		font-style: bold;
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
		font-size:1.2em;
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
		background-color: #3498db;
		color: E5E5E5;
		border: none;
		border-radius: 5px;
		cursor: pointer;
		font-size:1.2em;
		font-weight: bold;
	}
	.custom-alert button:hover {
		background-color: #2980b9;
	}
	#SizeList {
		text-align: center;
	}
	#fileList {
		display: flex;
	  flex-direction: column;
	  align-items: middle;
	  justify-content: space-around;
	}
  </style>  
</head>
<body>
  <div class="main">
  <h1>Upload de fichier</h1>
    <fieldset class="green">
    <legend>Upload de fichier</legend>
    <form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data">
      <p><input id="fileInput" type="file" name="file"></p>
      <p><input type="submit" value="Upload"></p>
    </form>
    <progress id="progressBar" value="0" max="100"></progress>
  </fieldset>
  <fieldset class="yellow">
    <legend>Espace LittleFS</legend>
    <p><button onclick="cleanResult()" class="cleanButton">Formater</button> <span id="cleanResult"></span></p>
    <pre id="SizeList"></pre>
  </fieldset>
  <fieldset class="red">
    <legend>Fichiers dans LittleFS</legend>
    <pre id="fileList"></pre>
  </fieldset>
  </div>
  <div class="overlay" id="overlay"></div>
  <div class="custom-alert" id="customAlert">
    <p id="alerte"></p>
    <p id="redalerte"></p>
    <p><button onclick="closeAlert()">Fermer</button></p>
  </div>
  <script>
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
    function showAlert(message, redmessage) {
      document.getElementById('customAlert').style.display = 'block';
      document.getElementById('overlay').style.display = 'block';
      document.getElementById('alerte').innerHTML = message;
      document.getElementById('redalerte').innerHTML = redmessage;
    }
    function closeAlert() {
      document.getElementById('customAlert').style.display = 'none';
      document.getElementById('overlay').style.display = 'none';
    }
    const form = document.getElementById('uploadForm');
    const progressBar = document.getElementById('progressBar');
    progressBar.style.display = 'none';
    const SizeList = document.getElementById('SizeList');
    const fileList = document.getElementById('fileList');
    var freeSpace;
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
        showAlert('etat: ', xhr.status)
        if (xhr.status === 200) {          
          showAlert('File upload succes', '');
          const url = '/FSorOTA';
          window.location.href = url;
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
          files.forEach(file => {
            const fileDiv = document.createElement('div');
            fileDiv.textContent = file;
            SizeList.appendChild(fileDiv);
            fileDiv.classList.add('SizeList');
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
            const deleteButton = document.createElement('button');
            deleteButton.textContent = 'Effacer';
            deleteButton.onclick = () => {
              const url = `/effacer?file=${encodeURIComponent(file)}`;
              window.location.href = url;
            };
            deleteButton.classList.add('deleteButton');
            fileDiv.appendChild(deleteButton);
            fileList.appendChild(fileDiv);
            fileDiv.classList.add('filelist');
          });
        }
      };
      xhr.send();
    }
  window.onload = function(){
    checkSizeFS();
    loadSizeFS();
    loadFileList();
  }
  </script>
</body>
</html>
)=====";
