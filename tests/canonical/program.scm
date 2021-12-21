(let((id (lambda(input dummy) (dummy input input))))
  (let((zero (id (lambda(z) z) id)))
    (let((one (id (lambda(succ z)(succ z)) id)))
      zero)))