import { renderer } from './renderer.js'

let x = 0
let y = 0
let xReal = 0
let yReal = 0

let clickX = 0
let clickY = 0
let isNewClick = false
let isMouseDown = false
let scroll = 0

export const mouse = {
    showCursor: function (cursor = '+') {
        renderer.drawObject(cursor, xReal, yReal)
    },
    info: function () {
        const isNew = isNewClick

        if (isNewClick) {
            isNewClick = false
        }

        const click = isNew ? {
            x: Math.floor(clickX / (window.fsize * 0.66)),
            y: Math.floor(clickY / (window.fsize * 1.22)),
            new: isNew
        } : null

        const tempScroll = scroll
        scroll = 0

        return {
            x: xReal,
            y: yReal,
            down: isMouseDown,
            click: click,
            scroll: tempScroll
        }

    },
    tick: function (event) {
        x = event.clientX
        y = event.clientY

        xReal = Math.floor(x / (window.fsize * 0.66))
        yReal = Math.floor(y / (window.fsize * 1.22))
    },
    click: function (event) {
        isNewClick = true

        // console.log(event)

        clickX = event.x
        clickY = event.y
    },

    onMouseUp: function () {
        // console.log('up')
        isMouseDown = false
    },
    onMouseDown: function () {
        // console.log('down')
        isMouseDown = true
    },
    wheel: function (e) {
        scroll = e.deltaY > 0 ? 1 : -1
    },
    init: function () {
        window.addEventListener('mousemove', this.tick)
        window.addEventListener('click', this.click)
        window.addEventListener('mousedown', this.onMouseDown)
        window.addEventListener('mouseup', this.onMouseUp)
        window.addEventListener('wheel', this.wheel)
    }
}