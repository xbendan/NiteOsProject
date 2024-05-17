template <class _Tp>
concept Pure = requires(_Tp t) {
    // clang-format off
    { t.filter() };
    { t.map() };
    { t.flatMap() };
    { t.findFirst() };
    { t.findLast() };
    { t.forEach() };
    { t.reduce() };
    { t.reduceRight() };
    { t.collect() };
    // clang-format on
};