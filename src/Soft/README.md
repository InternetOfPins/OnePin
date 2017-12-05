# OnePin Soft-devices

Soft device files should not contain a namespace as they will be included on a framework namespace providing specific instantiations and access to framework specific utils.

If mixing base frameworks then a specification of namespace must be provided

```c++
OneLib::Avr::Debouncer<...>;
OneLib::Arduino::Debouncer<...>;
```

## Debounce

Add software debounce to a pin

## Wire

Software wire, transfer state between two pins.
