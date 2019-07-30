'use strict'

const binding = process.electronBinding('demo')

module.exports = class Demo {
  constructor () {
    return binding.constructor()
  }
}
