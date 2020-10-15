set(srcs "src/sim7000g.c")

idf_component_register(SRCS "${srcs}"
                        INCLUDE_DIRS include
                        REQUIRES driver)