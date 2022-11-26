import { Point } from "./point.js"
import { Rectangle } from "./rectangle.js"

export class QuadTree {
    constructor(boundary, capacity) {
        this.boundary = boundary
        this.capacity = capacity

        this.points = []
        this.isSubdivided = false

        this.rectId = 0
    }

    subdivide() {
        const x = this.boundary.x
        const y = this.boundary.y
        const w = this.boundary.w
        const h = this.boundary.h

        let TLBoundary = new Rectangle(x, y, w / 2, h / 2)
        let TRBoundary = new Rectangle(x + w / 2, y, w / 2, h / 2)
        let BLBoundary = new Rectangle(x, y + w / 2, w / 2, h / 2)
        let BRBoundary = new Rectangle(x + w / 2, y + w / 2, w / 2, h / 2)

        this.TL = new QuadTree(TLBoundary, this.capacity)
        this.TR = new QuadTree(TRBoundary, this.capacity)
        this.BL = new QuadTree(BLBoundary, this.capacity)
        this.BR = new QuadTree(BRBoundary, this.capacity)

        this.isSubdivided = true
    }

    getRectangleEnds(rect) {
        const { x, y, w, h } = rect
        let TL = new Point(x, y)
        let TR = new Point(x + w, y)
        let BL = new Point(x, y + h)
        let BR = new Point(x + w, y + h)

        TL.quadTreeLinker = this.rectId
        TR.quadTreeLinker = this.rectId
        BL.quadTreeLinker = this.rectId
        BR.quadTreeLinker = this.rectId

        return [TL, TR, BL, BR]
    }

    insertRectangle(rect) {
        let rectangleEnds = rect.getRectangleEnds()

        rectangleEnds.forEach(endPoint => {
            endPoint.rectLinker = this.rectId
            endPoint.rectData = rect
            this.insertPoint(endPoint)
        })

        this.rectId += 1
    }

    insertPoint(point) {
        if (!this.boundary.contains(point)) {
            return false
        }
        // If there is space in this quad tree and if doesn't have subdivisions, add the object here
        if (this.capacity > this.points.length) {
            this.points.push(point)
            return true
        }

        // Otherwise, subdivide and then add the point to whichever node will accept it
        if (!this.isSubdivided) {
            this.subdivide()
        }

        this.TL.insertPoint(point)
        this.TR.insertPoint(point)
        this.BL.insertPoint(point)
        this.BR.insertPoint(point)
    }

    queryRectangles(range, rectangles = []) {
        let points = this.queryPoints(range)

        // add all points that have rectangle data
        points.forEach(point => {
            if (point.rectLinker) {
                rectangles.push(point.rectData)
            }
        })

        // remove dupes
        // rectangles = rectangles.filter((rect, i) => {
        //     const rectString = JSON.stringify(rect)
        //     return i === rectangles.findIndex(otherRect => {
        //         return JSON.stringify(otherRect) === rectString
        //     })
        // })

        return rectangles
    }

    queryPoints(range, points = []) {
        // Automatically abort if the range does not intersect this quad
        if (!this.boundary.intersectsRectangle(range)) {
            return points
        }

        // Check objects at this quad level
        this.points.forEach(point => {
            if (range.contains(point)) {
                points.push(point)
            }
        })

        // Terminate here, if there are no children
        if (!this.isSubdivided) {
            return points
        }

        // Otherwise, add the points from the children
        points = this.TL.queryPoints(range, points)
        points = this.TR.queryPoints(range, points)
        points = this.BL.queryPoints(range, points)
        points = this.BR.queryPoints(range, points)

        return points
    }
}