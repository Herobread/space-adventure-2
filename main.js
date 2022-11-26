import { kb } from './lib/keyboard.js'
import { logger } from './lib/logger.js'
import { mouse } from './lib/mouse.js'
import { asciiMap, renderer } from './lib/renderer.js'
import { pages } from './pages.js'
import { Camera } from './objects/Camera.js'

// editable config //

const startPageId = 0
window.showPerformance = true

// end editable config //


let interval
const asciicontainer = document.getElementById('asciicontainer')

window.w = 10
window.h = 10

window.asciiScreen = asciicontainer

window.page = 'main'
window.currentPage = window.page
window.currentPageFunction = pages[startPageId].func
window.fps = pages[startPageId].fps

window.clock = 0

window.frt = 0
window.logic = 0
window.objects = 0
window.activeAnimations = 0


function resizer() {
    window.w = Math.floor(window.innerWidth / (window.fsize * 0.66))
    window.h = Math.floor(window.innerHeight / (window.fsize * 1.22)) + 1

    asciiMap.init()
}

window.onload = function () {
    asciiMap.init()
    pages[startPageId].init()
    resizer()

    window.addEventListener('resize', resizer, false)

    updateFps(window.fps)

    mouse.init()
    kb.init()

    interval = setInterval(main, 1000 / 1000)
}


export function updateFps(fps) {
    clearInterval(interval)

    console.log('updated fps to', fps)

    window.fps = fps
    window.renderTime = 1000 / fps
    interval = setInterval(main, window.renderTime)
}

function main() {
    const start = performance.now()

    renderer.render()

    if (window.currentPage == window.page) {
        const s = performance.now()

        window.currentPageFunction()
        logger.log('logic', performance.now() - s)

        if (window.clock % 200 === 1) {
            console.log('logic: ', logger.getLog('logic'))
        }
    } else {
        pages.forEach(page => {
            if (page.name === window.page) {
                updateFps(page.fps)
                window.currentPage = window.page
                window.currentPageFunction = page.func
                page.init()
            }
        })
    }

    const end = performance.now()

    logger.log('rendertime', end - start)

    window.frameTime = end - start

    if (window.clock % 20 === 0) {
        const frt = logger.getLog('rendertime')
        window.frt = frt
        const logic = logger.getLog('logic')
        window.logic = logic
    }
}