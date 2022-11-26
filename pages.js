import { game, initGame } from "./pages/main.js"
import { animationViewer, initAnimationViewer } from "./utilityPages/viewAnimations.js"
import { assetsViewer, initAssetsViewer } from "./utilityPages/viewAssets.js"

const fps = 144

export const pages = [
    {
        name: 'main',
        func: game,
        fps: fps,
        init: initGame
    },

    /// utility pages
    {
        name: 'utility-asset-viewer',
        func: assetsViewer,
        fps: fps,
        init: () => { initAssetsViewer() }
    },
    {
        name: 'utility-animation-viewer',
        func: () => { animationViewer() },
        fps: fps,
        init: () => { initAnimationViewer() }
    }
]