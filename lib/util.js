export function center(width) {
    return parseInt(window.w / 2 - width / 2)
}

export function checkIfPointInRectangle(x, y, x1, y1, x2, y2) {
    return (isBetween(x, x1, x2) && isBetween(y, y1, y2))
}

export function isBetween(a, min, max) {
    return (Math.min(min, max) <= a && a <= Math.max(min, max))
}

export function cropImg(img, width, height) {
    if (width === -1 || height === -1)
        return img

    let res = ''
    let _y = 0;
    let _x = 0;
    for (let i = 0; i < img.length; i++) {
        if (img.charAt(i) !== '\n') {
            if (_y < height && _x < width)
                res += img.charAt(i)
            _x++
        } else {
            res += '\n'
            _x = 0
            _y++
        }
    }
    return res
}

export function randomInRange(start, end) {
    return Math.floor(Math.random() * (end - start + 1) + start);
}

export function randomInRangeFloat(start, end) {
    start *= 100
    end *= 100
    return Math.floor(Math.random() * (end - start + 1) + start) / 100;
}

export function clamp(num, min, max) {
    return Math.min(Math.max(num, min), max)
}

export function repeatSymbol(symbol, n) {
    let symbol_ = symbol
    for (let index = 0; index < n - 1; index++) {
        symbol += symbol_
    }

    return symbol
}

export function getDimensions(text) {
    console.log(text)
    let rows = text.split('\n')
    console.log(rows)

    const height = rows.length

    let width = 0

    rows.forEach(row => {
        if (row.length > width) {
            width = row.length
        }
    })

    console.log(height, width)

    return {
        height: height,
        width: width
    }
}