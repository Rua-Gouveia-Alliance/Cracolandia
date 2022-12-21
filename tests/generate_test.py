with open("generated_test.in", "w") as test:
    vertices = int(input("Vertices? "))

    test.write(str(vertices) + "\n")
    test.write(str(vertices*(vertices-1)) + "\n")

    for i in range(1, vertices + 1):
        for j in range(1, vertices + 1):
            if (j != i):
                test.write(str(i) + " " + str(j) + " 1\n")

    with open("generated_test.out", "w") as result:
        result.write(str(vertices-1) + "\n")
    
