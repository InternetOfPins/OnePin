# OnePin Soft-devices


some usefull software devices, some might be applicable over certain frameworks or require any sort of resources.

## Modules

### Debounce

Add software debounce to a pin.

This requires the sense of passed time. As example `millis()` function.

So only available of some environments.

### Wire

Software wire, transfer state between two pins.

## details
Soft device files should not contain a namespace as they will be included on a framework namespace providing specific instantiations and access to framework specific utils.

If mixing base frameworks then a specification of namespace must be provided to disambiguate

```c++
OneLib::Arduino::Debouncer<...>;
```
