import { Point } from "./point.js"

export class Rectangle {
    constructor(x, y, w, h) {
        this.x = x
        this.y = y
        this.w = w
        this.h = h
    }

    contains(point) {
        return this.x <= point.x && point.x <= this.x + this.w &&
            this.y <= point.y && point.y <= this.y + this.h
    }

    intersectsRectangle(rectangle) {
        return (this.x <= rectangle.x + rectangle.w &&
            this.x + this.w >= rectangle.x &&
            this.y <= rectangle.y + rectangle.h &&
            this.y + this.h >= rectangle.y)
    }

    getRectangleEnds() {
        const { x, y, w, h } = this
        let TL = new Point(x, y)
        let TR = new Point(x + w, y)
        let BL = new Point(x, y + h)
        let BR = new Point(x + w, y + h)

        return [TL, TR, BL, BR]
    }
}