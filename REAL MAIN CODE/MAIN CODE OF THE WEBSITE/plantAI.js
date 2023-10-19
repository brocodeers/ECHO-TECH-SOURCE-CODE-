const apiKey = 'vEzFnxWOQyznbGcUGs8gRMkPJCAz8xZRewuGiqQFIm4kpSMLQO';
const apiUrl = 'vEzFnxWOQyznbGcUGs8gRMkPJCAz8xZRewuGiqQFIm4kpSMLQO';

document.getElementById('identifyButton').addEventListener('click', identifyPlant);

function identifyPlant() {
    const imageInput = document.getElementById('plantImage');
    const resultElement = document.getElementById('result');

    const imageFile = imageInput.files[0];
    if (!imageFile) {
        resultElement.innerText = 'Please select an image first.';
        return;
    }

    const formData = new FormData();
    formData.append('images', imageFile);

    fetch(apiUrl, {
        method: 'POST',
        body: formData,
        headers: {
            'Api-Key': apiKey,
            'content-Type' : multipart/formData
        },
    })
    .then(response => response.json())
    .then(data => {
        // Handle the API response here
        resultElement.innerText = JSON.stringify(data, null, 2);
    })
    .catch(error => {
        console.error(error);
        resultElement.innerText = 'Error identifying the plant.';
    });
}
