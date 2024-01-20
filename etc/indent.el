(let ((file (car argv)))
  (with-temp-buffer
    (insert-file-contents-literally file)
    (c++-mode)
    (setq indent-tabs-mode nil)
    (c-set-style "gnu")
    (indent-region (point-min) (point-max))
    (write-region (point-min) (point-max) file))) 
