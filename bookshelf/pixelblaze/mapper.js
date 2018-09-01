function (pixelCount) {
  var map = []
  
  var stripSize = 15
  
  // Top row
  for (i = 1; i <= stripSize; i++) {
    map.push([i, 0])
  }
  for (i = stripSize + 2; i <= (stripSize*2 + 1); i++) {
    map.push([i, 0])
  }
  
  // Middle row, backwards
  for (i = (stripSize*2 + 1); i >= stripSize + 2; i--) {
    map.push([i, 16])
  }
  for (i = stripSize; i >= 1; i--) {
    map.push([i, 16])
  }
  
  // Bottom row
  for (i = 1; i <= stripSize; i++) {
    map.push([i, 32])
  }
  for (i = stripSize + 2; i <= (stripSize*2 + 1); i++) {
    map.push([i, 32])
  }
  
  // Right column backwards
  for (i = (stripSize*2 + 1); i >= stripSize + 2; i--) {
    map.push([32, i])
  }
    for (i = stripSize; i >= 1; i--) {
    map.push([32, i])
  }
  
  // Middle column
  for (i = 1; i <= stripSize; i++) {
    map.push([16, i])
  }
  for (i = stripSize + 2; i <= (stripSize*2 + 1); i++) {
    map.push([16, i])
  }
  
    for (i = (stripSize*2 + 1); i >= stripSize + 2; i--) {
    map.push([0, i])
  }
    for (i = stripSize; i >= 1; i--) {
    map.push([0, i])
  }
  
  return map
}
