
let fs = require('fs')
let otrjsn = fs.readFileSync(__dirname + '/otr.json', 'utf8')
let comppri = fs.readFileSync(__dirname + '/components.pri.conf', 'utf8')
let otr = JSON.parse(otrjsn)

exports.process = function (owmodules) {
    for(let module in owmodules) {
        if (owmodules[module] === true) {
            if (otr[module] === '') {
                logger.crit('Build API : Ow module not found in otr.json. Module: ' + module)
                return 0
            }

            comppri += '\nHEADERS += $$PWD/' + otr[module] + '.h'
            comppri += '\nSOURCES += $$PWD/' + otr[module] + '.cpp'
            comppri += '\nDEFINES += OW_' + otr[module].toUpperCase() + '\n'
        }
    }

    fs.writeFileSync(__dirname + '/components.pri', comppri, 'utf8')
    return 1;
}
