import { renderer } from "../lib/renderer.js"

export class Camera {
    constructor(x, y) {
        this.x = x
        this.y = y
        this.xVel = 0
        this.yVel = 0
    }

    move(x = 0, y = 0) {
        this.x += x
        this.y += y
    }

    getPosition() {
        return {
            x: parseInt(this.x),
            y: parseInt(this.y)
        }
    }

    tick(pointer, keyboard) {
        if (keyboard.down['d']) {
            this.xVel += 0.015
        }
        if (keyboard.down['a']) {
            this.xVel -= 0.015
        }
        if (keyboard.down['s']) {
            this.yVel += 0.015
        }
        if (keyboard.down['w']) {
            this.yVel -= 0.015
        }
        if (keyboard.new['r']) {
            this.x = 0
            this.y = 0
            this.xVel = 0
            this.yVel = 0
        }
        if (keyboard.new['f']) {
            this.x = 50_000 - 100
            this.y = 50_000 - 100
            this.xVel = 0
            this.yVel = 0
        }

        renderer.drawObject(`Camera velocity: x:${this.xVel.toFixed(2)} y:${this.yVel.toFixed(2)}`, 5, 0)

        this.move(this.xVel, this.yVel)
        this.xVel *= 0.99
        this.yVel *= 0.99
    }
}