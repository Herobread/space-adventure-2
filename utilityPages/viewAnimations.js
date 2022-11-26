import { kb } from "../lib/keyboard.js"
import { mouse } from "../lib/mouse.js"
import { renderer } from "../lib/renderer.js"
import { ui } from "../lib/ui.js"
import { art } from "../art.js"
import { getDimensions, randomInRange, randomInRangeFloat } from "../lib/util.js"
import { navigation } from './nav.js'
import { animations } from "../lib/animations.js"

let id = 0
let animations_ = art.animations
let anims = []
let spreadX = 0.7
let spreadY = 0.6
let loop = false
let tickSpeed = 17
let moveSpeed = 8

export function initAnimationViewer() {
    console.log('init')

    for (const [key, value] of Object.entries(animations_)) {
        anims.push({
            name: `${key}`,
            sprites: animations_[key].sprites,
        })
    }
}

export function animationViewer() {
    const pointer = mouse.info()
    const keyboard = kb.info()

    ui.dropDown('Navigation', navigation, 5, 5, pointer)
    animations.render()

    const command = `animations.animate(art.animations.${anims[id].name}, x, y, randomInRangeFloat(${-spreadX}, ${spreadX}), -${spreadY}, { tickSpeed: ${tickSpeed}, moveSpeed: ${moveSpeed}, loop: ${loop} })`

    const configurations = [
        {
            content: `|`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: `X spread(${spreadX.toFixed(1)})`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: '+',
            pointer: pointer,
            onClick: () => { spreadX += 0.1 },
        },
        {
            content: '-',
            pointer: pointer,
            onClick: () => { spreadX -= 0.1 },
        },
        {
            content: `|`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: `Y spread(${spreadY.toFixed(1)})`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: '+',
            pointer: pointer,
            onClick: () => { spreadY += 0.1 },
        },
        {
            content: '-',
            pointer: pointer,
            onClick: () => { spreadY -= 0.1 },
        },
        {
            content: `|`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: 'switch loop',
            pointer: pointer,
            onClick: () => { loop = !loop },
        },
        {
            content: `|`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: `tick speed ${tickSpeed}`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: '+',
            pointer: pointer,
            onClick: () => { tickSpeed += 1 },
        },
        {
            content: '-',
            pointer: pointer,
            onClick: () => { tickSpeed -= 1 },
        },
        {
            content: `|`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: `move speed ${moveSpeed}`,
            pointer: pointer,
            onClick: () => { },
            disabled: true,
        },
        {
            content: '+',
            pointer: pointer,
            onClick: () => { moveSpeed += 1 },
        },
        {
            content: '-',
            pointer: pointer,
            onClick: () => { moveSpeed -= 1 },
        },
    ]

    ui.buttonRow(configurations, 20, 4)

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
            if (id + 1 < anims.length)
                id += 1
        },
        style: 'underlined'
    })

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
                navigator.clipboard.writeText(`art.animations.${anims[id].name}`)
            },
            style: 'underlined',
        },
        {
            content: 'command',
            pointer: pointer,
            onClick: () => {
                navigator.clipboard.writeText(command)
            },
            style: 'underlined',
        }
    ]

    ui.buttonRow(buttons, window.w / 2 - 22, window.h - 8)

    let text = `${id}. art.animations.${anims[id].name}, sprites:${anims[id].sprites.length}`
    renderer.drawObject(text, window.w / 2 - text.length / 2, window.h - 10)

    renderer.drawObject(command, window.w / 2 - command.length / 2, window.h - 3)
    // renderer.drawObject(anims[id].sprti, window.w / 2 - anims[id].width / 2, window.h / 2 - anims[id].height / 2)

    animations.animate(anims[id], window.w / 2, window.h / 2, randomInRangeFloat(-spreadX, spreadX), -spreadY, { tickSpeed: tickSpeed, moveSpeed: moveSpeed, loop: loop })


    animations.tick()
    animations.move()

    mouse.showCursor()
}