import { kb } from "../lib/keyboard.js"
import { mouse } from "../lib/mouse.js"
import { renderer } from "../lib/renderer.js"
import { ui } from "../lib/ui.js"
import { art } from "../art.js"
import { getDimensions } from "../lib/util.js"
import { navigation } from './nav.js'

let id = 0
let { textures } = art
let images = []

export function initAssetsViewer() {
    console.log('init')

    for (const [key, value] of Object.entries(textures)) {
        const { height, width } = getDimensions(textures[key].img)

        images.push({
            name: `${key}`,
            img: textures[key].img,
            height: height,
            width: width
        })
    }
}

export function assetsViewer() {
    const pointer = mouse.info()
    const keyboard = kb.info()

    ui.dropDown('Navigation', navigation, 5, 5, pointer)

    // renderer.drawObject(`${id} - img id`, 0, 0)

    ui.button({
        content: '<',
        x: window.w * 0.1,
        y: window.h / 2,
        pointer: pointer,
        onClick: () => {
            if (id - 1 >= 0)
                id -= 1
        },
        style: 'underlined'
    })
    ui.button({
        content: '>',
        x: window.w * 0.9,
        y: window.h / 2,
        pointer: pointer,
        onClick: () => {
            if (id + 1 < images.length)
                id += 1
        },
        style: 'underlined'
    })

    let text = `${id}. art.textures.${images[id].name}, w:${images[id].width} h:${images[id].height}`
    renderer.drawObject(text, window.w / 2 - text.length / 2, window.h - 10)
    renderer.drawObject(images[id].img, window.w / 2 - images[id].width / 2, window.h / 2 - images[id].height / 2)

    let buttons = [
        {
            content: 'copy:',
            pointer: pointer,
            onClick: () => {
                console.log('nononono')
            },
            disabled: true
        },
        {
            content: 'path',
            pointer: pointer,
            onClick: () => {
                navigator.clipboard.writeText(`art.textures.${images[id].name}.img`)
            },
            style: 'underlined',
        },
        {
            content: 'dimensions',
            pointer: pointer,
            onClick: () => {
                navigator.clipboard.writeText(`width:${images[id].width}, height:${images[id].height}`)
            },
            style: 'underlined',
        },
        {
            content: 'img',
            pointer: pointer,
            onClick: () => {
                navigator.clipboard.writeText(`${images[id].img}`)
            },
            style: 'underlined',
        }
    ]

    ui.buttonRow(buttons, window.w / 2 - 16, window.h - 8)

    mouse.showCursor()
}