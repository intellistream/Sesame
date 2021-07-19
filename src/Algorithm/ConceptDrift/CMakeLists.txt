#If parent classes are not directly called, we don't need to add them as sources here.
#Instead, we only need to add those children classes.
add_source_sesame(
        ConceptDriftDetection.cpp
        ConceptDriftAdapation.cpp
)