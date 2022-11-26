import { renderer } from "./renderer.js"

const debug = 0

export const animations = {
    activeAnimations: [],
    amount: 0,
    animate: async function (animation, x, y, xVel = 0, yVel = 0, params) {
        let { tickSpeed, moveSpeed, loop } = params || {}

        tickSpeed ??= 5
        moveSpeed ??= 1
        loop ??= 0

        if (debug)
            console.log(`added animation at ${x} ${y}, ts = ${tickSpeed}, ms = ${moveSpeed}, xVel = ${xVel}`)

        this.activeAnimations.push({
            sprites: animation.sprites,
            stage: 0,
            x: x,
            y: y,
            xVel: xVel,
            yVel: yVel,
            loop: loop,
            tickSpeed: tickSpeed,
            moveSpeed: moveSpeed,
        })
    },
    /**
     * this function will increase all animations stage by 1 
     * and removes animations that are far from screen
     */
    tick: async function () {
        window.activeAnimations = this.activeAnimations.length
        this.activeAnimations.map((animation, id) => {
            if (window.clock % animation.tickSpeed) {
                return animation
            }

            animation.stage += 1

            const { loop } = animation

            if (loop && animation.stage >= animation.sprites.length) {
                animation.stage = 0
            }

            if (animation.x > window.w || animation.x < 0 || animation.y > window.h || animation.y < 0) {
                this.activeAnimations.splice(id, 1)
            }

            if (animation.stage >= animation.sprites.length) {

                if (debug)
                    console.log(`removed animation ${id}`)

                this.activeAnimations.splice(id, 1)
            }

            return animation
        })
    },
    move: async function () {
        this.activeAnimations.map(animation => {

            const { xVel, yVel, moveSpeed } = animation

            animation.x += xVel / moveSpeed
            animation.y += yVel / moveSpeed

            return animation
        })
    },
    render: async function () {
        this.amount = this.activeAnimations.length
        this.activeAnimations.forEach(animation => {
            renderer.drawObject(animation.sprites[animation.stage], animation.x, animation.y)
        })
    }
}