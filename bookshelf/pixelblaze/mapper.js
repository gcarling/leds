function (pixelCount) {
  var map = []
  
  var stripSize = 15
  for (i = 1; i <= stripSize; i++) {
    map.push([i, 0])
    map.push([i, 16])
    map.push([i, 32])
  }
  for (i = stripSize + 2; i <= (stripSize*2 + 1); i++) {
    map.push([i, 0])
    map.push([i, 16])
    map.push([i, 32])
  }
  for (i = 1; i <= stripSize; i++) {
    map.push([0, i])
    map.push([16, i])
    map.push([32, i])
  }
  for (i = stripSize + 2; i <= (stripSize*2 + 1); i++) {
    map.push([0, i])
    map.push([16, i])
    map.push([32, i])
  }
  return map
}
