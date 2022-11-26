import { animations } from '../lib/animations.js'
import { kb } from '../lib/keyboard.js'
import { mouse } from '../lib/mouse.js'
import { colisions } from '../lib/colisions.js'
import { Rectangle } from '../lib/quadtree/rectangle.js'
import { QuadTree } from '../lib/quadtree/quadtree.js'
import { Point } from '../lib/quadtree/point.js'
import { randomInRange } from '../lib/util.js'
import { renderer } from '../lib/renderer.js'
import { shapes } from '../lib/shapes.js'
import { Camera } from '../objects/camera.js'

let boundary = new Rectangle(0, 0, 500_000, 500_000)

let qt = new QuadTree(boundary, 40)
let camera = new Camera(0, 0)

export function initGame() {
    for (let i = 0; i < 200000; i += 1) {
        let rect = new Rectangle(randomInRange(0, 50_000), randomInRange(0, 50_000), randomInRange(10, 50), randomInRange(10, 50))
        qt.insertRectangle(rect)
    }

    console.log(qt)
}

export function game() {
    const pointer = mouse.info()
    const keyboard = kb.info()
    const cameraPos = camera.getPosition()
    const cameraX = cameraPos.x
    const cameraY = cameraPos.y


    let range = new Rectangle(cameraX, cameraY, window.w, window.h)

    let rectangles = []
    rectangles = qt.queryRectangles(range)

    let minX = 10_000_000
    let minY = 10_000_000
    rectangles.forEach((rect, i) => {
        let tempRect = new Rectangle(rect.x - cameraX, rect.y - cameraY, rect.w, rect.h)

        if (minX > rect.x)
            minX = rect.x
        if (minY > rect.y)
            minY = rect.y

        renderer.drawObject(`Rectangle(${i}): x:${rect.x} y:${rect.y} w:${rect.w} h:${rect.h}`, rect.x - cameraX, rect.y - cameraY - 1)
        shapes.rectangle('@', tempRect)
    })

    camera.tick(pointer, keyboard)

    tick()
    mouse.showCursor()



    renderer.drawObject(`camera x ${cameraX} camera y ${cameraY} Amout of rectangle drawn:${rectangles.length} \nmin rectangle: x:${minX} min y:${minY}`, 5, window.h - 10)
    renderer.drawObject(`Press r to reset camera`, 5, window.h - 8)
    renderer.drawObject(`Press f to go to the bottom right`, 5, window.h - 7)
}

function tick() {
    colisions.check()

    animations.move()
    animations.tick()
    animations.render()
}