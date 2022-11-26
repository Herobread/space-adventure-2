let logs = {}

export const logger = {
    add: function (name) {
        logs[name] = []
    },
    checkForExistance: function (name) {
        if (!logs[name])
            this.add(name)
    },
    log: function (name, log) {
        this.checkForExistance(name)

        logs[name].push(log)
    },
    getLog: function (name, isConsoleLog) {
        this.checkForExistance(name)

        let total = 0
        let amount = logs[name].length

        logs[name].forEach(log => {
            total += log
        })

        if (isConsoleLog)
            console.log(`${name}: ${total / amount}(${amount})`)

        logs[name] = []

        return total / amount
    }
}