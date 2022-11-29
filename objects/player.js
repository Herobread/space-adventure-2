import { animations } from "../lib/animations.js"
import { renderer } from "../lib/renderer.js"
import { degreesToRadians, isBetween, randomInRangeFloat } from "../lib/util.js"
import { art } from '../art.js'

export class Player {
    constructor(x, y) {
        this.x = x
        this.y = y

        this.acceleration = 0.01

        this.velocity = 0
        this.rotationAcceleration = 0.05
        this.rVel = 0 // rotation velocity

        this.sprites = {
            up: {
                img: `  .  
 .'. 
 : : 
.' '.
:.-.:
'   '
`,
                width: 4,
                height: 5,
                engines: [
                    {
                        x: 1,
                        y: 5,
                    },
                    {
                        x: 5,
                        y: 5,
                    }
                ]
            },
            down: {
                img: `.   .
:'-':
'. .'
 : : 
 '.' 
  '  
`,
                width: 4,
                height: 5,
                engines: [
                    {
                        x: 1,
                        y: 0,
                    },
                    {
                        x: 5,
                        y: 0,
                    }
                ]
            },
            right: {
                img: `---._    
',   ''._
.'  _..' 
---'     
`,
                width: 8,
                height: 3,
                engines: [
                    {
                        x: 0,
                        y: 0,
                    },
                    {
                        x: 0,
                        y: 3,
                    }
                ]
            },
            left: {
                img: `    _.---
_.''   ,'
 '.._  '.
     '---
`,
                width: 8,
                height: 3,
                engines: [
                    {
                        x: 8,
                        y: 0,
                    },
                    {
                        x: 8,
                        y: 3,
                    }
                ]
            },
            //             topRight: {
            //                 img: `        .'
            //      .\`': 
            //   .''   : 
            // :'     :  
            // ''.  .'   
            //   '.:     
            // `,
            //                 width: 9,
            //                 height: 5
            //             },
            //             upLeft: {
            //                 img: `'.        
            //  :''.     
            //  :   ''.  
            //   :     ':
            //    '.  .''
            //      :.'  
            // `,
            //                 width: 9,
            //                 height: 5
            //             },
            //             downLeft: {
            //                 img: `      ..  
            //     :' '..
            //    .'    :
            //   :     .'
            //  :   .''  
            //  :..'     
            // .'        
            // `,
            //                 width: 9,
            //                 height: 6
            //             },
            //             downRight: {
            //                 img: `  ..       
            // ..' ':     
            // :    '.    
            // '.     :   
            //   ''.   :  
            //      '..:  
            //          '.
            // `,
            //                 width: 10,
            //                 height: 6
            //             }
        }

        this.rotation = 0
    }
    tick(pointer, keyboard) {
        if (keyboard.down['d']) {
            this.rVel -= this.rotationAcceleration
        }
        if (keyboard.down['a']) {
            this.rVel += this.rotationAcceleration
        }
        this.rVel *= 0.9

        this.rotation += this.rVel

        if (this.rotation < -45) {
            this.rotation += 360
        }
        if (this.rotation > 315) {
            this.rotation -= 360
        }

        if (keyboard.down['w']) {
            this.velocity += this.acceleration
            this.rotation = this.alignToAngle(this.rotation, 0.05)
        }

        if (keyboard.down['s']) {
            this.velocity -= this.acceleration
            this.rotation = this.alignToAngle(this.rotation, 0.05)
        }

        const frameXVelocity = this.velocity * Math.cos(degreesToRadians(this.rotation))
        const frameYVelocity = -this.velocity * Math.sin(degreesToRadians(this.rotation))
        this.x += frameXVelocity
        this.y += frameYVelocity

        this.velocity *= 0.99

        const sprite = this.selectSprite()
        renderer.drawObject(`${this.rotation} ${this.x} ${this.y}`, 5, 20)
        renderer.drawObject(sprite.img, window.w / 2 - sprite.width / 2, window.h / 2 - sprite.height / 2)
        sprite.engines.forEach(engine => {

            let particleXVelocity = -frameXVelocity// + randomInRangeFloat(-0.03, 0.03)
            let particleYVelocity = -frameYVelocity// + randomInRangeFloat(-0.03, 0.03)
            let particleX = window.w / 2 - sprite.width / 2 + engine.x
            let particleY = window.h / 2 - sprite.height / 2 + engine.y

            animations.animate(art.animations.particle, particleX, particleY, particleXVelocity, particleYVelocity)

            particleXVelocity = -frameXVelocity + randomInRangeFloat(-0.03, 0.03)
            particleYVelocity = -frameYVelocity + randomInRangeFloat(-0.03, 0.03)
            particleX = window.w / 2 - sprite.width / 2 + engine.x
            particleY = window.h / 2 - sprite.height / 2 + engine.y

            animations.animate(art.animations.particle, particleX, particleY, particleXVelocity, particleYVelocity)

        })
    }
    getXY() {
        return {
            x: this.x,
            y: this.y
        }
    }
    selectSprite() {
        //     if (isBetween(this.rotation, -30, 30))
        //         return this.sprites.right
        //     if (isBetween(this.rotation, 30, 60))
        //         return this.sprites.topRight
        //     if (isBetween(this.rotation, 60, 120))
        //         return this.sprites.up
        //     if (isBetween(this.rotation, 120, 150))
        //         return this.sprites.upLeft
        //     if (isBetween(this.rotation, 150, 210))
        //         return this.sprites.left
        //     if (isBetween(this.rotation, 210, 240))
        //         return this.sprites.downLeft
        //     if (isBetween(this.rotation, 240, 300))
        //         return this.sprites.down
        //     if (isBetween(this.rotation, 300, 330))
        //         return this.sprites.downRight


        if (isBetween(this.rotation, -45, 45))
            return this.sprites.right
        if (isBetween(this.rotation, 45, 135))
            return this.sprites.up
        if (isBetween(this.rotation, 135, 225))
            return this.sprites.left
        if (isBetween(this.rotation, 225, 315))
            return this.sprites.down
    }
    alignToAngle(number, modifier) {
        if (isBetween(number, -45, 0))
            return number + modifier
        if (isBetween(number, 0, 45))
            return number - modifier

        if (isBetween(number, 45, 90))
            return number + modifier
        if (isBetween(number, 90, 135))
            return number - modifier

        if (isBetween(number, 135, 180))
            return number + modifier
        if (isBetween(number, 180, 225))
            return number - modifier

        if (isBetween(number, 225, 270))
            return number + modifier
        if (isBetween(number, 270, 315))
            return number - modifier
    }
}

/*

                           
                           
---._                      
',   ''._                  
.'  _..'                   
---'                       



*/