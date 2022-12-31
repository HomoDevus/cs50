window.addEventListener('load', () => {
    const LIGHT_ELEM = document.querySelector("#light")
    const TIMINGS = [100, 200, 500, 1000, 1500]

    for (let timing of TIMINGS) {
        setTimeout(() => {
            LIGHT_ELEM.classList.toggle("glow")
        }, timing)
    }
})