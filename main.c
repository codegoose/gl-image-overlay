#define NANOVG_GL3_IMPLEMENTATION

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <nanovg.h>
#include <nanovg_gl.h>
#include <stb_image.h>
#include <stb_image_write.h>

int main() {

    // Load the image

    int width, height, num_components;

    stbi_uc *image_data = stbi_load("C:/Users/Brandon/Sync/wallpapers/wallhaven-83qyry.jpg", &width, &height, &num_components, 4);

    printf("Image @ %p (%d by %d, %d components)\n", image_data, width, height, num_components);

    // Initialize OpenGL context

    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(200, 200, "GLFW3", 0, 0); // Size of this window doesn't matter since I'm not using the backbuffer
    glfwMakeContextCurrent(window);

    // Load additional OpenGL functions

    glewExperimental = GL_TRUE;
    glewInit();
    
    // Initialize vector graphics library

    NVGcontext *nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

    printf("Font #%d\n", nvgCreateFont(nvg, "default", "C:/Windows/Fonts/CALIFB.TTF"));

    // Create off-screen rendering surface

    printf("Preparing for render...\n");

    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    // Upload reference picture to GPU

    int image_texture = nvgCreateImageRGBA(nvg, width, height, 0, image_data);

    printf("Image texture #%d\n", image_texture);

    // Render

    printf("Rendering...\n");

    glViewport(0, 0, width, height);

    nvgBeginFrame(nvg, width, height, 1);
    nvgBeginPath(nvg);
    nvgRect(nvg, 0, 0, width, height);
    nvgFillPaint(nvg, nvgImagePattern(nvg, 0, 0, width, height, 0, image_texture, 1));
    nvgFill(nvg);
    nvgBeginPath(nvg);
    nvgRect(nvg, 0, height - 260, width, 260);
    nvgFillColor(nvg, nvgRGBA(8, 8, 8, 128));
    nvgFill(nvg);
    nvgFontSize(nvg, 80);
    nvgFillColor(nvg, nvgRGB(255, 255, 255));
    nvgText(nvg, 100, height - 100, "This amazing font was rendered on the GPU.", 0);
    nvgEndFrame(nvg);

    // Download result from GPU

    unsigned char *buffer = malloc(width * height * 4);
    unsigned char *flipped_buffer = malloc(width * height * 4);
    
    printf("Downloading image from GPU...\n");

    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    printf("Flipping image...\n");

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            for (int k = 0; k < 4; k++)
                flipped_buffer[(i + j * width) * 4 + k] = buffer[(i + (height - 1 - j) * width) * 4 + k];

    printf("Saving image to disk...\n");

    stbi_write_jpg("output.jpg", width, height, STBI_rgb_alpha, flipped_buffer, 0);

    // Cleanup

    printf("Cleaning up...");

    free(flipped_buffer);
    free(buffer);

    nvgDeleteImage(nvg, image_texture);
    glDeleteRenderbuffers(1, &renderbuffer);
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &framebuffer);
    nvgDeleteGL3(nvg);
    glfwTerminate();

    return 0;
}