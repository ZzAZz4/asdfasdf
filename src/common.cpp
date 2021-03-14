
template<template<class, class...> class Container, class T, class... U>
bool contains (const Container<T, U...>& c, const T& elem)
{
    return c.find(elem) != c.end();
}