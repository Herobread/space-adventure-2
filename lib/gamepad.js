const debug = 0

export const gamepad = {
    test: function (id = 0) {
        const gamepads = navigator.getGamepads()
        // todo: add every button
        if (gamepads[id]) {
            let buttons = {
                axes: {
                    x1: gamepads[id].axes[0],
                    y1: gamepads[id].axes[1],
                    x2: gamepads[id].axes[2],
                    y2: gamepads[id].axes[3],
                },
                buttons: {
                    cross: gamepads[0].buttons[0].pressed
                }
            }
            return buttons
        }
        return null
    }
}