const options = {
	method: 'GET',
	headers: {
		'X-RapidAPI-Key': '07bdd9bb9cmsha0dd03a76b40f5cp10cdbfjsn3e5fbbf2bb3d',
		'X-RapidAPI-Host': 'weather-by-api-ninjas.p.rapidapi.com'
	}
};

const getweather = (city)=> {
    cityName.innerHTML = city
    fetch('https://weather-by-api-ninjas.p.rapidapi.com/v1/weather?city=' + city, options)
    .then(response => response.json())
    .then(response => {
        console.log(response)
        cloud_pct.innerHTML = response.cloud_pct
        temp.innerHTML = response.temp
        feels_like.innerHTML = response.feels_like
        min_temp.innerHTML = response.min_temp
        max_temp.innerHTML = response.max_temp
        wind_speed.innerHTML = response.wind_speed
        sunrise.innerHTML = response.sunrise
        sunset.innerHTML = response.sunset
        humidity.innerHTML = response.humidity
        
    })
    .catch(err => console.error(err));
}

submit.addEventListener("click", (e) => {
    e.preventDefault()
    getweather(city.value)
})

getweather("Chandigarh")