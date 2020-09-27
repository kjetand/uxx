# TODO
- [x] Implement a `named_type` ala https://github.com/joboccara/NamedType
      in order to support strongly typed input params.
- [ ] It should not be allowed to pass uxx-objects through optional args
- [ ] Properties for checkbox (label placement)
- [ ] IDs should be passed as a `uxx::explicit_arg` (confusing IDs vs. labels)
- [ ] Report/fix internal compiler crash when using implicit type argument deduction for
      alias templates (e.g. `uxx:max{0.1f}` rather than `uxx::max<float>{0.1f}`)
- [ ] Consider to create uxx::tab instead of using uxx::window. Alternatively,
      rename uxx::window to something generic.