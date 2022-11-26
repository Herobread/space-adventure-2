import { logger } from "./logger.js"

window.fsize = 10
window.clock = 0
let screen = Array(window.w).fill(null).map(() => Array(window.h))

export const asciiMap = {
    putSymbol: function (char, x, y) {
        if (x >= 0 && x < window.w && y >= 0 && y < window.h && screen)
            screen[y][x] = char
    },
    getSymbol: function (x, y) {
        return screen[y][x]
    },
    init: function () {
        screen = Array(window.w).fill(null).map(() => Array(window.h))
    },
}

export const renderer = {
    put: async function (text) {
        window.asciiScreen.textContent = text
    },
    drawSymbol: function (symbol, x, y) {
        if (!symbol) {
            throw 'Object to draw is not defined'
        }
        if (x == undefined) {
            throw `X is not defined(${object})`
        }
        if (y == undefined) {
            throw `Y is not defined(${object})`
        }

        asciiMap.putSymbol(symbol[0], x, y)
    },
    drawObject: async function (object, x, y) {
        if (!object) {
            throw 'Object to draw is not defined'
        }
        if (x == undefined) {
            throw `X is not defined(${object})`
        }
        if (y == undefined) {
            throw `Y is not defined(${object})`
        }

        let _y = 0;
        let _x = 0;
        x = parseInt(x)
        y = parseInt(y)
        for (let i = 0; i < object.length; i++) {
            if (object.charAt(i) != '\n') {
                asciiMap.putSymbol(object.charAt(i), x + _x, y + _y)
                _x++
            } else {
                _x = 0
                _y++
            }
        }
    },
    drawTransparentObject: async function (object, x, y) {
        let _y = 0;
        let _x = 0;
        x = parseInt(x)
        y = parseInt(y)
        for (let i = 0; i < object.length; i++) {
            if (object.charAt(i) === ' ') {
                _x++
            } else if (object.charAt(i) !== '\n') {
                asciiMap.putSymbol(object.charAt(i), x + _x, y + _y)
                _x++
            } else {
                _x = 0
                _y++
            }
        }
    },
    clear: async function () {
        asciiMap.init()
    },
    render: async function () {
        if (window.showPerformance) {
            this.drawObject(`Page ${window.page}.js h=${window.h} w=${window.w}, \nFrame render time = ${window.frt.toFixed(2)} ms${(1000 / window.frt).toFixed(2)}, logic time = ${window.logic.toFixed(2)}, (${(window.logic / window.frt * 100).toFixed(2)}%)
Active animations:${window.activeAnimations}, objects: ${window.objects}, colision obj: ${window.colisionObjects}`, 5, 5)
        }

        const start = performance.now()

        window.clock += 1

        if (window.clock === 1001) {
            window.clock = 0
        }

        let res = ''

        for (let y = 0; y < window.h; y += 1) {
            for (let x = 0; x < window.w; x += 1) {
                const symbol = asciiMap.getSymbol(x, y)

                if (symbol)
                    res += symbol
                else
                    res += ' '
            }
            res += '\n'
        }
        this.put(res)
        this.clear()

        const end = performance.now()

        logger.log('render', end - start)

        if (window.clock % 200 == 0) {
            console.log(logger.getLog('render'))
        }
    }
}
