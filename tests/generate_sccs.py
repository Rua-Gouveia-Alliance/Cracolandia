with open("generated_test.in", "w") as test:
    sccs = int(input("SCCS? "))

    test.write(str(sccs*3) + "\n")
    test.write(str(sccs*3) + "\n")

    j = 1
    for i in range(sccs):
        test.write(str(j) + " " + str(j+1) + " 1\n")
        test.write(str(j) + " " + str(j+2) + " 1\n")
        test.write(str(j+1) + " " + str(j+2) + " 1\n")
        j += 3;

    with open("generated_test.out", "w") as result:
        result.write(str(sccs*2) + "\n")
