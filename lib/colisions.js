let objects = []

export const colisions = {
    checkPointInSquare: function (x1, y1, x, y, w, h) {
        x1 = Math.floor(x1)
        y1 = Math.floor(y1)
        x = Math.floor(x)
        y = Math.floor(y)
        w = Math.floor(w)
        h = Math.floor(h)

        const checkWidth = x <= x1 && x1 < x + w
        const checkHeight = y <= y1 && y1 < y + h

        // console.log(x1, x, x + w, y1, y, y + h, checkHeight && checkWidth)

        if (checkHeight && checkWidth) {
            return true
        }
        return false
    },
    checkRectanglesColision: function (rectangle1, rectangle2) {
        return (
            rectangle1.x < rectangle2.x + rectangle2.w &&
            rectangle1.x + rectangle1.w > rectangle2.x &&
            rectangle1.y < rectangle2.y + rectangle2.h &&
            rectangle1.h + rectangle1.y > rectangle2.y
        )
    },
    distance: function (point1, point2) {
        return Math.sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y))
    },

    addRectangleColision: function ({ w, h, x, y }, name, onColision) {
        objects.push({
            onColision: onColision,
            name: name,
            w: w,
            h: h,
            x: x,
            y: y
        })
    },
    check: function () {
        // sorting by x 
        window.colisionObjects = objects.length

        objects.forEach((object1, i) => {
            objects.forEach((object2, ii) => {
                if (object1 !== object2) {
                    if (this.checkRectanglesColision(object1, object2)) {
                        object1.onColision(object2.name)
                        object2.onColision(object1.name)
                    }
                }
            })
        })

        objects = []
    }
}