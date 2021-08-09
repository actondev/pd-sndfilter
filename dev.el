(defun pd-sndfilter/compile ()
  (interactive)
  (let ((default-directory (projectile-project-root)))
    (compile "meson compile -C build"))) ;; add --verbose for debugging

(transient-define-prefix pd-sndfilter ()
  :transient-suffix     'transient--do-call
  ["Quit"
   ("q" "quit" transient-quit-all)
   ("Q" "quit & restore windows" aod-do/restore :transient transient--do-quit-all)]
  ["Commands"
   ("c" "compile" pd-sndfilter/compile)
   ])
