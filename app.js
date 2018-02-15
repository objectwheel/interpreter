let fs = require('fs')
let components = require(__dirname + '/components/components.js')
let qtrjsn = fs.readFileSync(__dirname + '/qtr.json', 'utf8')
let apppri = fs.readFileSync(__dirname + '/app.pri.conf', 'utf8')
let qtr = JSON.parse(qtrjsn)

exports.process = function(build, projectname) {
    if (build.versioncode === '' ||
        build.qtmodules === '' ||
        build.owmodules === '') {
        logger.crit('Build API : build.json file is corrupted.')
        return 0
    }

    apppri += 'VERSION = ' + build.versioncode
    apppri += '\nTARGET = "' + projectname + '"'

    let qtmodules = build.qtmodules
    for(let module in qtmodules) {
        if (qtmodules[module] === true) {
            if (qtr[module] === '') {
                logger.crit('Build API : Qt module not found in qtr.json. Module: ' + module)
                return 0
            }

            apppri += '\nQT += ' + qtr[module]
        }
    }

    fs.writeFileSync(__dirname + '/app.pri', apppri, 'utf8')
    return components.process(build.owmodules)
}

