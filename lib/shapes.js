import { renderer } from './renderer.js'

export const shapes = {
    /**
     * @param  {string} symbol symbol will be used in drawing
     * @param  {number} x1 coordinate
     * @param  {number} y1 coordinate
     * @param  {number} x2 coordinate
     * @param  {number} y2 coordinate
     */
    line: function (symbol, x1, y1, x2, y2) {
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        x1 = parseInt(x1)
        x2 = parseInt(x2)
        y1 = parseInt(y1)
        y2 = parseInt(y2)

        let dx = Math.abs(x2 - x1)
        let sx = x1 < x2 ? 1 : -1
        let dy = -Math.abs(y2 - y1)
        let sy = y1 < y2 ? 1 : -1
        let error = dx + dy

        let end = false
        while (!end) {
            renderer.drawObject(symbol, x1, y1)
            if (x1 == x2 && y1 == y2)
                end = true
            let e2 = 2 * error
            if (e2 >= dy) {
                if (x1 == x2)
                    end = true
                error = error + dy
                x1 = x1 + sx
            }
            if (e2 <= dx) {
                if (y1 == y2)
                    end = true
                error = error + dx
                y1 = y1 + sy
            }
        }
    },
    /**
     * @param  {string} symbol symbol will be used in drawing
     * @param  {number} x1 coordinate
     * @param  {number} y1 coordinate
     * @param  {number} x2 coordinate
     * @param  {number} y2 coordinate
     */
    rectangle: function (symbol, rect) {
        const x1 = parseInt(rect.x)
        const y1 = parseInt(rect.y)
        const x2 = parseInt(rect.x + rect.w)
        const y2 = parseInt(rect.y + rect.h)

        this.line(symbol, x1, y1, x1, y2)
        this.line(symbol, x1, y2, x2, y2)
        this.line(symbol, x2, y1, x2, y2)
        this.line(symbol, x1, y1, x2, y1)
    },
    /**
     * @param  {string} symbol symbol will be used in drawing
     * @param  {number} x1 coordinate
     * @param  {number} y1 coordinate
     * @param  {number} x2 coordinate
     * @param  {number} y2 coordinate
     */
    ellipse: function (symbol, x, y, a, b) {
        a = Math.abs(x - a)
        b = Math.abs(y - b)
        let xlast = x;
        let ylast = y;

        for (var angle = 0; angle <= 720; angle++) {
            let X = parseInt(x + (a * Math.cos(angle * 2 * (Math.PI / 720))) + 0.5);
            let Y = parseInt(y + (b * Math.sin(angle * 2 * (Math.PI / 720))) + 0.5);
            if (xlast != X || ylast != Y) {
                xlast = X; ylast = Y;
            }
            renderer.drawObject(symbol, X, Y)
        }
    }
}